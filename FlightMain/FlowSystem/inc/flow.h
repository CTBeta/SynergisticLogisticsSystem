/*
 * Flow System based on PX4FLOW by PX4 Development Team
 * Developed by Jason.C.T Mar.2016
 */
 
#ifndef FLOW_H_
#define FLOW_H_

#include <stdint.h>

/**
 * @brief Computes pixel flow from image1 to image2
 */
uint8_t compute_flow(uint8_t *image1, uint8_t *image2, float x_rate, float y_rate, float z_rate,
		float *histflowx, float *histflowy);

#endif /* FLOW_H_ */
