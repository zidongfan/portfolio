package com.example.synthesizer;

import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.AnchorPane;

public class SineWaveWidget extends AudioComponentWidget{
    SineWaveWidget(AudioComponent ac, AnchorPane parent, String name) {
        super(ac, parent, name);
        super.setLayoutX(50);
        super.setLayoutY(200);

        nameLabel_.setText("Sine Wave (440 Hz)");
        Slider slider = new Slider(220, 880, 440);
        slider.setOnMouseDragged(e -> handleSlider(e, slider, nameLabel_));
        center.getChildren().add(slider);

        baseLayout.getChildren().remove(leftSide);

    }

    private void handleSlider(MouseEvent e, Slider slider, Label nameLabel_) {
        freValue_ = (int) slider.getValue();
        nameLabel_.setText("Sine Wave (" + freValue_ + ")");
    }

    public AudioComponent getAudioComponent() {
        super.audioComponent_.setValue(freValue_);
        return super.audioComponent_;
    }

    private int freValue_ = 440;

}


