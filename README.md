# Stereo Channel Splitter

Stereo Channel Splitter is a simple C program that splits the channels of a stereo audio file, allowing the listener to appreciate sounds typically mixed between the left and right channels. This program converts a stereo WAV file to PCM format, performs stereo-to-mono conversion, and then converts the resulting PCM file back to WAV format.

## How to Use

1. **Build the program:**

   Before using the Stereo Channel Splitter, you need to compile the C program. Open a terminal and navigate to the directory containing the source code (`s16le.c`). Then, use `gcc` to compile the program:

   ```bash
   gcc -o s16le s16le.c -Wall
   ```

2. **Convert the WAV file to PCM format:**
    After building the program, you need to convert your stereo WAV file to PCM format. You can use ffmpeg for this task. For example:
    ```bash
    ffmpeg -i "input.wav" -f s16le "output.pcm"
    ```
    Replace "input.wav" with the path to your original WAV file and "output.pcm" with the desired name for the PCM file.

3. **Use the splitter:**
    Once you have the PCM file, you can use the Stereo Channel Splitter to split the channels. Run the following command in the terminal:
    ```bash
    ./s16le "input.pcm" "output-split.pcm"
    ```
    Replace "input.pcm" with the path to your PCM file and "output-split.pcm" with the desired name for the split PCM file.

4. **Convert the splitter PCM file to WAV:**
    After splitting the channels, you can convert the resulting PCM file back to WAV format using ffmpeg. Run the following command:
    ```bash
    ffmpeg -ac 2 -ar 44.1k -f s16le -i "output-split.pcm" "output-split.wav"
    ```
    Replace "output-split.pcm" with the path to your split PCM file and "output-split.wav" with the desired name for the split WAV file.

5. **Listen to the Split Channels:**
    Once you have the split WAV file, you can listen to the split channels using any audio player that supports WAV format.

## Note

    Make sure to replace the file names and paths with your actual file names and paths.
    This program assumes that the input file is in signed 16-bit little-endian format (s16le).
    Feel free to adjust any details or add additional information as needed!

## License

This project is licensed under the Public Domain.

## Author

stereosplit is developed by Abel Romero Pérez.
