/*
 * Copyright (c) 2021 Espressif Systems (Shanghai) Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <soc/soc_memory_layout.h>
#include <zephyr/multi_heap/shared_multi_heap.h>
#include "esp_heap_caps.h"
#include <zephyr/drivers/dma.h>
#include <zephyr/device.h>
#include <zephyr/random/random.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

#define ALLOCATE_BYTES 10

/**
 * @brief This function allocates a buffer to the internal RAM memory
 * 
 * @param buffer 
 * @return int 
 */
int allocate_buffer(uint8_t** buffer, int bytes)
{
	*buffer = k_malloc(bytes);

	if (*buffer == NULL){
		LOG_ERR("buffer allocation failed %p", (void*)*buffer);
		return -1;
	} else {
		LOG_INF("buffer allocated at: %p", (void*)*buffer);
	}

	return 0;
}

void fill_buffer_with_random_content(uint8_t *buffer, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        *(buffer + i) = sys_rand8_get();  // Fill buffer with random integers
	LOG_DBG("[%d] %p", i, (void*)&buffer[i]);
    }
}

bool verify_buffer_content(const uint8_t *buffer1, const uint8_t *buffer2, size_t size)
{
	LOG_DBG("verify_buffer_content:");
   	for (size_t i = 0; i < size; i++) {
		LOG_DBG("%d => [%p] %d - [%p] %d", i, (void*)&buffer1[i], buffer1[i], (void*)&buffer2[i], buffer2[i]);
		if (buffer1[i] != buffer2[i]) {
			return false;  // Mismatch found
		}
	}
    return true;
}

void dma_callback(const struct device *dma_dev, void *arg, uint32_t channel, int status)
{
    if (status == 0) {
        LOG_INF("DMA transfer complete on channel %d", channel);
    } else {
        LOG_ERR("DMA transfer failed with status %d on channel %d", status, channel);
    }
}

int configure_dma_transfer(const struct device *dma_dev, uint8_t *src, uint8_t *dest, size_t size, uint8_t channel)
{
	struct dma_config dma_cfg = {0};
	struct dma_block_config block_cfg = {0};

	block_cfg.source_address = (uint32_t)src;
	block_cfg.dest_address = (uint32_t)dest;
	block_cfg.block_size = size;

	LOG_DBG("source: 0x%x, dest: 0x%x", block_cfg.source_address, block_cfg.dest_address);

	dma_cfg.channel_direction = MEMORY_TO_MEMORY;
	dma_cfg.source_burst_length = 4;  // Assuming 4 bytes per burst
	dma_cfg.dest_burst_length = 4;
	dma_cfg.source_data_size = 4;  // 4 bytes (32 bits)
	dma_cfg.dest_data_size = 4;
	dma_cfg.block_count = 1;
	dma_cfg.head_block = &block_cfg;
	dma_cfg.dma_callback = dma_callback;

	LOG_INF("Configuring DMA...");

	int ret = dma_config(dma_dev, channel, &dma_cfg);
	if (ret != 0) {
		LOG_ERR("DMA configuration failed with error %d\n", ret);
		return ret;
	}

	LOG_INF("DMA configured");

	ret = dma_start(dma_dev, channel);
	if (ret != 0) {
		LOG_ERR("Failed to start DMA transfer with error %d\n", ret);
	}

	LOG_INF("DMA started");

	return ret;
}

int main(void)
{
	uint8_t *buffer1, *buffer2;
	const struct device *dma_dev = DEVICE_DT_GET(DT_ALIAS(dma0));
	struct dma_status dma_status = {0};
	int ret = 0;
	uint8_t tx_dma_channel = 2;

	// Allocate the buffers
	allocate_buffer(&buffer1, ALLOCATE_BYTES);
	allocate_buffer(&buffer2, ALLOCATE_BYTES);

	// Fulfill the source buffer with random content
	fill_buffer_with_random_content(buffer1, ALLOCATE_BYTES);
	
	if (!device_is_ready(dma_dev)){
		LOG_ERR("NULL DMA device");
	} else {
		LOG_INF("DMA device is ready");	
	}

	ret = configure_dma_transfer(dma_dev, buffer1, buffer2, ALLOCATE_BYTES, tx_dma_channel);
	if (ret != 0) {
		LOG_ERR("DMA configuration or transfer failed\n");
		return ret;
	}

	// Wait for transfer completion (in real applications, you'd wait for the callback)
	k_sleep(K_MSEC(100));  // Simulate waiting for DMA completion

	if (verify_buffer_content(buffer1, buffer2, ALLOCATE_BYTES)) {
		LOG_INF("Buffer content matches\n");
	} else {
		LOG_ERR("Buffer content mismatch\n");
	}

	ret = dma_get_status(dma_dev, tx_dma_channel, &dma_status);
	if (ret != 0){
		LOG_ERR("Error getting DMA status");
	}

	return 0;
}
