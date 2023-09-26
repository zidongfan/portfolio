package com.example.synthesizer;

import java.util.Arrays;

public class AudioClip {

    public static final double duration = 2.0; // 2 seconds
    public static final int sampleRate = 44100; //44100 respectively

    public static final short numSamples = (short) 88200; // Total sample is 88200;
    byte[] data = new byte[(int) (2 * numSamples)]; // a member variable that contains the actual byte array (176400)

    // convert the 2 bytes data to a short sample
    public short getSample(int index) {
        short first = (short) (data[2 * index] & 0xFF);
        short second = (short) (data[2 * index + 1] << 8);
        return (short) (first | second);
    }

    // set the sample, convert the input short value to 2 bytes data;
    public void setSample(int index, short value) {
        data[2 * index] = (byte) (value & 0xFF);
        data[2 * index + 1] = (byte) (value >> 8);
    }

    // get the byte data array
    public byte[] getData() {
        return Arrays.copyOf(data, data.length);
    }



    // Tests
    public static void main(String[] args) {
        AudioClip audioClip = new AudioClip();
        audioClip.setSample(0, Short.MAX_VALUE);
        audioClip.setSample(1, Short.MIN_VALUE);
        audioClip.setSample(2, (short) 0);
        audioClip.setSample(3, (short) 1);
        audioClip.setSample(4, (short) -1);
        System.out.println(audioClip.getSample(0));
        System.out.println(audioClip.getSample(1));
        System.out.println(audioClip.getSample(2));
        System.out.println(audioClip.getSample(3));
        System.out.println(audioClip.getSample(4));
    }


}