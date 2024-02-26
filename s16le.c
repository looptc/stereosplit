/*
 * Stereo Channel Splitter
 *
 * This program takes an input stereo audio file and splits the two channels,
 * allowing the listener to appreciate sounds that are typically mixed between
 * the left and right channels. It performs stereo-to-mono conversion by
 * calculating the average of the left and right channels for the left output
 * channel and the difference of the left and right channels for the right
 * output channel.
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// Define file permission mode for output file
#define OUT_FILE_MODE   (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

// Define constants for channel configurations
#define OUT_CHN 2
#define OUT_CHS (sizeof(short))
#define INP_CHN 2
#define INP_CHS (sizeof(short))
#define CH_LEFT_OF 0
#define CH_RIGHT_OF 1

int main(int argc, char **argv) {
    // File descriptors for input and output files
    int fdin = -1;
    int fdout = -1;

    // Structure to hold input file information
    struct stat stin;
    memset(&stin, 0, sizeof(stin));

    // Pointers to input and output memory mappings
    short *min = NULL;
    short *mout = NULL;

    // Variables for audio sample processing
    short il = 0, ir = 0, ol = 0, or = 0;
    int ctmp = 0;
    size_t szin = 0, szout = 0, samples = 0;
    off_t ofin = 0, ofout = 0;

    // Check if correct number of command-line arguments provided
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_s16le_2ch> <output_s16le_2ch>\n", argv[0]);
        return -1;
    }

    // Open input file
    fdin = open(argv[1], O_RDONLY);
    if (fdin == -1) {
        fprintf(stderr, "Error opening input file %s: %s\n", argv[1], strerror(errno));
        return errno;
    }

    // Open or create output file
    fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, OUT_FILE_MODE);
    if (fdout == -1) {
        close(fdin);
        fprintf(stderr, "Error opening output file %s: %s\n", argv[2], strerror(errno));
        return errno;
    }

    // Retrieve input file size
    fstat(fdin, &stin);
    szin = stin.st_size;

    // Map input file into memory
    min = mmap(NULL, szin, PROT_READ, MAP_SHARED, fdin, 0);
    if (min == MAP_FAILED) {
        close(fdin);
        close(fdout);
        fprintf(stderr, "Error mapping input file %s: %s\n", argv[1], strerror(errno));
        return errno;
    }

    // Set output file size to match input file size
    szout = szin;
    ftruncate(fdout, szout);

    // Map output file into memory
    mout = mmap(NULL, szout, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, 0);
    if (mout == MAP_FAILED) {
        munmap(min, szin);
        close(fdin);
        close(fdout);
        fprintf(stderr, "Error mapping output file %s: %s\n", argv[2], strerror(errno));
        return errno;
    }

    // Calculate number of audio samples
    samples = szin / INP_CHS;

    // Process audio samples
    while (ofin != samples) {
        // Extract left and right channel samples
        il = min[ofin + CH_LEFT_OF];
        ir = min[ofin + CH_RIGHT_OF];

        // Perform stereo-to-mono conversion
        ctmp = il + ir;
        ol = ctmp >> 1;
        ctmp = il - ir;
        or = ctmp >> 1;

        // Write converted samples to output
        mout[ofout + CH_LEFT_OF] = ol;
        mout[ofout + CH_RIGHT_OF] = or;

        // Move to next audio samples
        ofin += INP_CHN;
        ofout += OUT_CHN;
    }

    // Clean up: unmap memory mappings and close file descriptors
    munmap(min, szin);
    close(fdin);
    close(fdout);
    munmap(mout, szout);

    return 0;
}
