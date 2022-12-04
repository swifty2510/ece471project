#ifndef DEVICES
#define DEVICES

int init_devices(int *SPI_fd, struct spi_ioc_transfer *spi, u_int8_t *data_out, u_int8_t *data_in, int *I2C_fd);
int shutdown_devices(int *SPI_fd, int *I2C_fd);
double read_ADC(int SPI_fd, struct spi_ioc_transfer *spi, u_int8_t *data_in, u_int8_t *data_out);

#endif
