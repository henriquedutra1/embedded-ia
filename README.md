# Embedded AI on STM32 and Zynq

My work for the Embedded AI course at MINES Saint-Étienne: neural networks trained on MNIST with Keras, converted to C and run on embedded targets: an STM32L4R9I-DISCO with X-CUBE-AI (STM32Cube.AI), and the ARM core of a Zynq-7020 (ZedBoard) with the open-source AIfES library.

| Project | Model | Weights size | Accuracy (Keras, 10k images) | Board (100 images) |
|---|---|---|---|---|
| [MNIST on an STM32L4R9 with X-CUBE-AI](MNIST%20on%20an%20STM32L4R9%20with%20X-CUBE-AI) | Tiny CNN, 6,478 parameters | 8.1 KB | 95.77 % | 99/100 |
| [MNIST MLP on an STM32L4R9 with X-CUBE-AI](MNIST%20MLP%20on%20an%20STM32L4R9%20with%20X-CUBE-AI) | 5-layer MLP, 575,050 parameters (8-bit weight compression) | 593 KiB | 97.75 % | 100/100 |
| [MNIST MLP on a Zynq with AIfES](MNIST%20MLP%20on%20a%20Zynq%20with%20AIfES) | Same MLP, AIfES F32 and Q7 | 2.19 MiB (F32) / 564 KiB (Q7) | 97.75 % | in progress |

Each folder has its own README with the training, the conversion, the embedded code and the results.
