package com.example.synthesizer;

import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.Clip;
import javax.sound.sampled.LineUnavailableException;
import java.util.ArrayList;

public interface AudioComponent {

    AudioClip getClip(); // return the current sound produced by this component

    boolean hasInput(); // determine can connect AudioComponent to this as an input

    void connectInput(AudioComponent input); // connect the input and store the reference of the AudioComponent parameter, if the component doesn't accept inputs, assert (false)

    void removeInput(AudioComponent input);

    void setValue(double value);

    // AudioComponent Sinewave
    class SineWave implements AudioComponent {

        private AudioComponent audioComponent;
        private double frequency_;

        // SineWave constructor use input frequency
        public SineWave(double frequency) {
            frequency_ = frequency;
        }

        // set the AudioClip of SineWave in here
        @Override
        public AudioClip getClip() {
            AudioClip audioClip = new AudioClip();
            SineWave sineWave = new SineWave(440);
            int maxValue = Short.MAX_VALUE;
            for (int i = 0; i < AudioClip.numSamples; i++) {
                audioClip.setSample(i, (short) (maxValue * Math.sin(2 * Math.PI * frequency_ * i / AudioClip.sampleRate)));
            }
            return audioClip;
        }

        // This component can not input something
        @Override
        public boolean hasInput() {
            return false;
        }

        // This component can not accept any input
        @Override
        public void connectInput(AudioComponent input) {
            assert (false);
        }

        @Override
        public void removeInput(AudioComponent input) {
            assert (false);
        }

        @Override
        public void setValue(double value) {
            frequency_ = value;
        }

    }

    // AudioComponent Filters to change the volume of input AudioComponent
    class Filters implements AudioComponent {
        private AudioComponent input_;
        private double scale_; // scale >= 0    if scale = 0, mute; if scale < 1 , decrease the volume; if scale > 1 increase the volume;

        // Filter constructor with set scale
        public Filters(double scale) {
            this.scale_ = scale;
        }

        @Override
        public AudioClip getClip() {


            AudioClip original = input_.getClip();
            AudioClip result = new AudioClip();
            for (int i = 0; i < AudioClip.numSamples; i++) {
                double value = scale_ * original.getSample(i);
                // set the max and min value after change
                if (value < Short.MIN_VALUE) {
                    value = Short.MIN_VALUE;
                } else if (value > Short.MAX_VALUE) {
                    value = Short.MAX_VALUE;
                }
                result.setSample(i, (short) value);
            }
            return result;
        }

        @Override
        public boolean hasInput() {
            return (input_ != null);
        }

        @Override
        public void connectInput(AudioComponent input) {
            input_ = input;
        }

        @Override
        public void removeInput(AudioComponent input) {
            input_ = null;
        }

        @Override
        public void setValue(double value) {
            scale_ = value;
        }

    }


    // AudioComponent Mixers
    class Mixers implements AudioComponent {
        // set ArrayList of AudioComponent and store the input AudioComponents in here
        ArrayList<AudioComponent> audioComponents = new ArrayList<>(5);

        // the combine AudioClip that stored mixed data;
        private AudioClip combine_ = new AudioClip();

        // two for loops to combine the all audio components value
        @Override
        public AudioClip getClip() {
            for (int i = 0; i < audioComponents.size(); i++) {
                AudioClip temp = audioComponents.get(i).getClip();

                for (int j = 0; j < AudioClip.numSamples; j++) {
                    int sum = combine_.getSample(j) + temp.getSample(j);
                    // set the boundary of value
                    if (sum < Short.MIN_VALUE) {
                        sum = Short.MIN_VALUE;
                    } else if (sum > Short.MAX_VALUE) {
                        sum = Short.MAX_VALUE;
                    }

                    combine_.setSample(j, (short) sum);
                }

            }
            return combine_;
        }

        @Override
        public boolean hasInput() {
            return (audioComponents != null);
        }

        // add teh input AudioComponent to the ArrayList
        @Override
        public void connectInput(AudioComponent input) {
            audioComponents.add(input);
        }

        @Override
        public void removeInput(AudioComponent input) {
            audioComponents.remove(input);
        }

        @Override
        public void setValue(double value) {
            assert (false);
        }

        class LinearRamp implements AudioComponent {
            private AudioComponent input_;
            private final int start_ = 50;
            private final int stop_ = 2000;

            // set the Linear Ramp in here
            @Override
            public AudioClip getClip() {
                AudioClip linearClip = input_.getClip();
                for (int i = 0; i < AudioClip.numSamples; i++) {
                    linearClip.setSample(i, (short) ((start_ * (AudioClip.numSamples - i) + stop_ * i) / AudioClip.numSamples));
                }
                return linearClip;
            }

            @Override
            public boolean hasInput() {
                return (input_ != null);
            }

            @Override
            public void connectInput(AudioComponent input) {
                input_ = input;
            }

            @Override
            public void removeInput(AudioComponent input) {
                input_ = null;
            }

            @Override
            public void setValue(double value) {
                assert (false);
            }

        }

        class VFSineWave implements AudioComponent {
            private AudioComponent input_;

            @Override
            public AudioClip getClip() {
                AudioClip get = new AudioClip();
                int phase = 0;
                for (int i = 0; i < AudioClip.numSamples; i++) {
                    phase += 2 * Math.PI * input_.getClip().getSample(i) / AudioClip.numSamples;
                    get.setSample(i, (short) (Short.MAX_VALUE * Math.sin(phase)));
                }
                return get;
            }

            @Override
            public boolean hasInput() {
                return (input_ != null);
            }

            // only input linear ramp to VFSineWave
            @Override
            public void connectInput(AudioComponent input) {
                this.input_ = input;
            }

            @Override
            public void removeInput(AudioComponent input) {
                input_ = null;
            }

            @Override
            public void setValue(double value) {
                assert (false);
            }
        }







        // play SineWave sound
        public static void main(String[] args) throws LineUnavailableException {
            // SineWave

            // Get properties from the system about samples rates, etc.
            // AudioSystem is a class from the Java standard library.
            Clip c = AudioSystem.getClip(); // Note, this is different from our AudioClip class.
            // This is the format that we're following, 44.1 KHz mono audio, 16 bits per sample.
            AudioFormat format16 = new AudioFormat(44100, 16, 1, true, false);

            AudioComponent sineWave = new SineWave(440); // Your code
            AudioClip clip = sineWave.getClip();         // Your code

            c.open(format16, clip.getData(), 0, clip.getData().length); // Reads data from our byte array to play it.

            System.out.println("About to play SineWave Test");
            c.start(); // Plays it.
            c.loop(2); // Plays it 2 more times if desired, so 6 seconds total

            // Makes sure the program doesn't quit before the sound plays.
            while (c.getFramePosition() < AudioClip.numSamples || c.isActive() || c.isRunning()) {
                // Do nothing while we wait for the note to play.
            }

            System.out.println("Done.");
            c.close();


            // SineWave connect to Filter
            Clip f = AudioSystem.getClip();
            AudioComponent filter = new Filters(0.5F);

            filter.connectInput(sineWave);
            AudioClip fclip = filter.getClip();

            f.open(format16, fclip.getData(), 0, fclip.getData().length);

            System.out.println("About to play Filter Test");
            f.start(); // Plays it.
            f.loop(2); // Plays it 2 more times if desired, so 6 seconds total

            // Makes sure the program doesn't quit before the sound plays.
            while (f.getFramePosition() < AudioClip.numSamples || f.isActive() || f.isRunning()) {

            }

            System.out.println("Done.");
            f.close();

            // test for Mixers
            Clip m = AudioSystem.getClip();
            AudioComponent sineWave2 = new SineWave(220);
            AudioComponent mixer = new Mixers();
            mixer.connectInput(sineWave);
            mixer.connectInput(sineWave2);
            AudioClip mclip = mixer.getClip();

            m.open(format16, mclip.getData(), 0, mclip.getData().length);

            System.out.println("About to play Mixer Test");
            m.start(); // Plays it.
            m.loop(2); // Plays it 2 more times if desired, so 6 seconds total

            // Makes sure the program doesn't quit before the sound plays.
            while (m.getFramePosition() < AudioClip.numSamples || m.isActive() || m.isRunning()) {

            }

            System.out.println("Done.");
            m.close();

        }

    }
}

