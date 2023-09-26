package com.example.synthesizer;

import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.AnchorPane;

public class FilterWidget extends AudioComponentWidget{
    FilterWidget(AudioComponent ac, AnchorPane parent, String name) {
        super(ac, parent, name);
        super.setLayoutX(200);
        super.setLayoutY(100);

        // Label name and slider
        nameLabel_.setText("Filter ( 50% )");
        Slider slider = new Slider(0, 100, 50);
        slider.setOnMouseDragged(e -> handleSlider(e, slider, nameLabel_));
        center.getChildren().add(slider);
    }

    private void handleSlider(MouseEvent e, Slider slider, Label nameLabel_) {
        value = (int) slider.getValue();
        nameLabel_.setText("Filter ( " + value + "% )");
    }

    public AudioComponent getAudioComponent() {
        System.out.println("get the Filter ");
        super.audioComponent_.setValue(value*0.01);
        if (super.audioComponent_.hasInput() == false && widgetIamConnected_ != null) {
            super.audioComponent_.connectInput(widgetIamConnected_.getAudioComponent());
        }
        return super.audioComponent_;
    }



    private int value = 50;
}
