#include "../../fftw-3.3.10/api/fftw3.h"
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "devices.h"

#define FFTBUFFER 1024

u_int8_t append(double *adc_val_in, u_int8_t *current_entry,  u_int16_t adcVal, u_int8_t *filled){

	adc_val_in[*current_entry] = adcVal;
	*current_entry = *current_entry +1;
	if(*current_entry  == FFTBUFFER){
		*current_entry = 0;
		*filled = 1;
	}
	return *filled;
}

int main(int argc, char **argv) {

	int SPI_fd,I2C_fd;
	double adcVal;
	u_int8_t i2cbuffer[17];

	struct spi_ioc_transfer spi;
        u_int8_t data_out[3];
        u_int8_t data_in[3];

	//fft variables
	u_int8_t current_entry =0;
	u_int8_t filled=0;
	u_int32_t n_out = ((FFTBUFFER/2) +1);
	double *adc_val_in;
	fftw_complex *fft_out;

	//initilize SPI bus for ADC transfers and I2C bus for two displays
	if( init_devices(&SPI_fd, &spi, data_out, data_in, &I2C_fd) < 0){
		fprintf(stderr, "Error initilizing I2C or SPI\n");
		return -1;
	}

	//setup for fourier transforms

	//create arrays
	adc_val_in = (double*)fftw_malloc(sizeof(double) * FFTBUFFER);
	fft_out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * n_out);

	// append input array with ADC values, compute transform, display magnitudes
	while(1){

		//read adc val from SPI bus
		adcVal = read_ADC(SPI_fd, &spi, data_in, data_out);
		if(adcVal == -1) {
			fprintf(stderr, "Error reading adc\n");
			return -1;
		}

		//append array
		filled = append(adc_val_in, &current_entry, adcVal, &filled);

		if(filled){
			//create plan, real to complex 1 dimensional
			fftw_plan plan;
			plan = fftw_plan_dft_r2c_1d( FFTBUFFER, adc_val_in, fft_out, FFTW_ESTIMATE);
			//execute plan
			fftw_execute(plan);

			//refill array
			filled = 0;

		}

	}


	//Shut down SPI and I2C devices
	if( shutdown_devices(&SPI_fd, &I2C_fd) < 0){
		fprintf(stderr, "Failed to close SPI or I2C fd\n");
		return -1;
	}
	return 0;
}

