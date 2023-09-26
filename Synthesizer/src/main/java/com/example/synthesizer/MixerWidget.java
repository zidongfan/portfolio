package com.example.synthesizer;

import javafx.scene.layout.AnchorPane;

import javax.sound.sampled.Mixer;

public class MixerWidget extends AudioComponentWidget{
    MixerWidget(AudioComponent ac, AnchorPane parent, String name) {
        super(ac, parent, name);
        super.setLayoutX(200);
        super.setLayoutY(300);

        nameLabel_.setText("Mixer");

    }
    public AudioComponent getAudioComponent() {
        System.out.println("get the Mixer");
        if (widgetIamConnected_ != null) {
            super.audioComponent_.connectInput(widgetIamConnected_.getAudioComponent());
        }
        return super.audioComponent_;
    }


}
