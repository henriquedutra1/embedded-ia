import serial
import numpy as np

PORT = "COM8"


def synchronise_UART(serial_port):
    """Send 0xAB until the STM32 answers 0xCD."""
    while True:
        serial_port.write(b"\xAB")
        ret = serial_port.read(1)
        if ret == b"\xCD":
            serial_port.read(1)
            break


def send_inputs_to_STM32(inputs, serial_port):
    """Send one 28x28 image as float32 bytes."""
    inputs = inputs.astype(np.float32)
    buffer = b""
    for x in inputs:
        buffer += x.tobytes()
    serial_port.write(buffer)


def read_output_from_STM32(serial_port):
    """Read the 10 class scores (uint8) and scale them back to [0, 1]."""
    output = serial_port.read(10)
    return [int(out) / 255 for out in output]


def evaluate_model_on_STM32(iterations, serial_port):
    accuracy = 0
    for i in range(iterations):
        print(f"----- Iteration {i+1} -----")
        send_inputs_to_STM32(X_test[i], serial_port)
        output = read_output_from_STM32(serial_port)
        if np.argmax(output) == np.argmax(Y_test[i]):
            accuracy += 1 / iterations
        print(f"   Expected output: {Y_test[i]}")
        print(f"   Received output: {output}")
        print(f"----------------------- Accuracy: {accuracy:.2f}\n")
    return accuracy


if __name__ == '__main__':
    X_test = np.load("../model/MNIST_xtest_NN_C2_16_10.npy")
    Y_test = np.load("../model/MNIST_ytest_NN_C2_16_10.npy")

    with serial.Serial(PORT, 115200, timeout=1) as ser:
        print("Synchronising...")
        synchronise_UART(ser)
        print("Synchronised")

        print("Evaluating model on STM32...")
        error = evaluate_model_on_STM32(100, ser)
