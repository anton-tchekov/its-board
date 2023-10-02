/**
 * @file    spi_ll.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    2023-09-25
 * @brief   SPI low-level interface
 */

#ifndef __SPI_LL_H__
#define __SPI_LL_H__

void spi_ll_init(void);
int spi_ll_xchg(int val);
void spi_ll_fast(void);
void spi_ll_slow(void);

#endif /* __SPI_LL_H__ */
