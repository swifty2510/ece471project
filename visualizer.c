#include <errno.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {

	int SPI_fd,I2C_fd1,I2C_fd2;

	u_int8_t i2cbuffer[17];

	struct spi_ioc_transfer spi;
        u_int8_t data_out[3];
        u_int8_t data_in[3];

	/* Open SPI device */
        u_int8_t spi_mode = SPI_MODE_0;
	SPI_fd= open("/dev/spidev0.0", O_RDWR);
        if(SPI_fd ==-1) fprintf(stderr,"Error opening SPI device\n\r");
        /* set spi mode to 0*/
        if( ioctl(SPI_fd, SPI_IOC_WR_MODE,&spi_mode) <0) fprintf(stderr,"error setting spi mode\n\r");

	/* set up SPI for transfer of 3 bytes(full duplex)*/
        memset(&spi,0,sizeof(struct spi_ioc_transfer));
        spi.tx_buf = (u_int32_t)&data_out;
        spi.rx_buf = (u_int32_t)&data_in;
        spi.len = 3;
        spi.delay_usecs = 0;
        spi.speed_hz = 1350000; //1.35MHz for 3.3V or 3.6MHz for 5v
        spi.bits_per_word = 8;
        spi.cs_change = 0;

	/* Open I2C devices */
	I2C_fd1 = open("/dev/i2c-1", O_RDWR);
	if(I2C_fd1 == -1) fprintf(stderr, "Error opening I2C device 1\n\r");

	I2C_fd2 = open("/dev/i2c-2", O_RDWR);
	if(I2C_fd2 == -1) fprintf(stderr, "Error opening I2C device 2\n\r");

	/* Set I2C addresses */
	if(ioctl(I2C_fd1, I2C_SLAVE, 0x70) < 0) fprintf(stderr, "error setting I2C 1 address 0x70\n");
	if(ioctl(I2C_fd2, I2C_SLAVE, 0x71) < 0) fprintf(stderr, "error setting I2C 2 address 0x71\n");

	

}
