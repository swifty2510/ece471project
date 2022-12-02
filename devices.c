#include <unistd.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

int init_devices(int *SPI_fd, struct spi_ioc_transfer *spi, u_int8_t *data_out, u_int8_t *data_in, int *I2C_fd){
	/* Open SPI device */
        u_int8_t spi_mode = SPI_MODE_0;
        *SPI_fd= open("/dev/spidev0.0", O_RDWR);
        if(*SPI_fd ==-1) fprintf(stderr,"Error opening SPI device\n\r");
        /* set spi mode to 0*/
        if( ioctl(*SPI_fd, SPI_IOC_WR_MODE,&spi_mode) <0) fprintf(stderr,"error setting spi mode\n\r");

        /* set up SPI for transfer of 3 bytes(full duplex)*/
        memset(spi,0,sizeof(struct spi_ioc_transfer));
        spi->tx_buf = (u_int32_t)&data_out;
       	spi->rx_buf = (u_int32_t)&data_in;
        spi->len = 3;
        spi->delay_usecs = 0;
        spi->speed_hz = 1350000; //1.35MHz for 3.3V or 3.6MHz for 5v
        spi->bits_per_word = 8;
        spi->cs_change = 0;

        /* Open I2C devices */
        *I2C_fd = open("/dev/i2c-1", O_RDWR);
        if(*I2C_fd == -1){
		fprintf(stderr, "Error opening I2C device 1\n\r");
		return -1;
	}


        /* Set I2C addresses */
        if(ioctl(*I2C_fd, I2C_SLAVE, 0x72) < 0){
		 fprintf(stderr, "error setting I2C 1 address 0x72\n");
		return -1;
	}
        if(ioctl(*I2C_fd, I2C_SLAVE, 0x75) < 0){
		 fprintf(stderr, "error setting I2C 2 address 0x75\n");
		return -1;
	}

	return 0;

}

int shutdown_devices(int *SPI_fd, int *I2C_fd){
	if( close(*SPI_fd) < 0){
		fprintf(stderr,"Failed to close SPI fd\n");
		return -1;
	}

	if( close(*I2C_fd) <0){
		fprintf(stderr,"Failed to close I2C fd\n");
		return -1;
	}
	return 0;
}
