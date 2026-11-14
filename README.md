# Embedded AI on STM32

My work for the Embedded AI course at MINES Saint-Étienne: neural networks trained on MNIST with Keras, converted to C with X-CUBE-AI (STM32Cube.AI), run on an STM32L4R9I-DISCO board, and checked from a PC over UART.

| Project | Model | Flash (weights) | Accuracy (Keras, 10k images) | Board (100 images) |
|---|---|---|---|---|
| [MNIST on an STM32L4R9 with X-CUBE-AI](MNIST%20on%20an%20STM32L4R9%20with%20X-CUBE-AI) | Tiny CNN, 6,478 parameters | 8.1 KB | 95.77 % | 99/100 |
| [MNIST MLP on an STM32L4R9 with X-CUBE-AI](MNIST%20MLP%20on%20an%20STM32L4R9%20with%20X-CUBE-AI) | 5-layer MLP, 575,050 parameters (8-bit weight compression) | 593 KiB | 97.75 % | 100/100 |

Each folder has its own README with the training, the CubeMX analysis, the firmware, the PC test script and the results.
