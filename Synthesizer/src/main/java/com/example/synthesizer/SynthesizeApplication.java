package com.example.synthesizer;

import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.stage.Stage;

import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.Clip;
import javax.sound.sampled.LineUnavailableException;
import java.io.IOException;
import java.util.ArrayList;


public class SynthesizeApplication extends Application {
    @Override
    public void start(Stage stage) throws IOException {
        BorderPane root = new BorderPane();
        Scene scene = new Scene(root, 800, 600);

        stage.setTitle("My Synthesize");

        // Rightr
        VBox rightPanel = new VBox();
        rightPanel.setPadding(new Insets(4));
        rightPanel.setStyle("-fx-background-color: darkgray");


        // Sine Wave button
        Button sineWaveBtn = new Button("Sine Wave");
        rightPanel.getChildren().add(sineWaveBtn);
        sineWaveBtn.setOnAction(e -> createSineWave("SineWave"));

        // Filter button
        Button filterBtn = new Button("Filter");
        rightPanel.getChildren().add(filterBtn);
        filterBtn.setOnAction(e -> createFilter("Filter"));

        // Mixer button
        Button mixerBtn = new Button("Mixer");
        rightPanel.getChildren().add(mixerBtn);
        mixerBtn.setOnAction(e -> createMixer("Mixer"));

        // Center Panel Stuff;
        mainCanvas_ = new AnchorPane();
        mainCanvas_.setStyle("-fx-background-color: darkgray");


        speaker_ = new Circle(600, 300, 15); // Speaker Circle
        speaker_.setFill(Color.BLACK);
        mainCanvas_.getChildren().add(speaker_);

        VBox volume_ = new VBox(); // Volume Control
        volume_.setStyle("-fx-border-color: black; -fx-border-image-width: 8; -fx-background-color: white");
        Label volumeLabel = new Label("Volume: 50");
        Slider volumeSlider = new Slider(0, 100, 50);
        volumeSlider.setOnMouseDragged(e -> handleVolumeSlider(e, volumeSlider, volumeLabel));
        volume_.getChildren().add(volumeLabel);
        volume_.getChildren().add(volumeSlider);
        volume_.setAlignment(Pos.CENTER);
        volume_.setPadding(new Insets(5));
        volume_.setSpacing(5);
        volume_.setLayoutX(10);
        volume_.setLayoutY(10);

        mainCanvas_.getChildren().add(volume_);


        // Bottom
        HBox bottomPane = new HBox();
        bottomPane.setAlignment(Pos.CENTER);
        Button playBtn = new Button("Play");
        playBtn.setOnAction(e -> playNetwork());
        bottomPane.getChildren().add(playBtn);

        // Top
        Label title = new Label("Simple Synthesizer");


        // Put the panels into the BorderPane(Root)
        root.setRight(rightPanel);
        root.setCenter(mainCanvas_);
        root.setBottom(bottomPane);
        root.setTop(title);

        stage.setScene(scene);
        stage.show();
    }

    private void handleVolumeSlider(MouseEvent e, Slider volumeSlider, Label volumeLabel) {
        volumeValue_ = (int) volumeSlider.getValue();
        volumeLabel.setText("Volume: " + volumeValue_);
    }


    private void playNetwork() {
        if (widgets_.size() == 0) {
            return;
        }

        try {

            Clip c = AudioSystem.getClip();
            AudioListener listener = new AudioListener(c);

            AudioComponent.Mixers mixer = new AudioComponent.Mixers();
            for (AudioComponentWidget w : widgets_) {
                AudioComponent ac = w.getAudioComponent();
                mixer.connectInput(ac);
            }

            AudioComponent.Filters filter = new AudioComponent.Filters((float) (volumeValue_ * 0.01));
            filter.connectInput(mixer);

            byte[] data = filter.getClip().getData();

            AudioFormat format = new AudioFormat(44100, 16, 1, true, false);

            c.open(format, data, 0, data.length);
            c.start();

            c.addLineListener(listener);
        } catch (LineUnavailableException e) {

        }

    }


    private void createSineWave(String sineWave) {
        System.out.println("in create Sine Wave component widget");
        AudioComponent sineWaveCW = new AudioComponent.SineWave(440);
        AudioComponentWidget acw = new SineWaveWidget(sineWaveCW, mainCanvas_, "Sine Wave");
        existWidgets_.add(acw);
    }

    private void createFilter(String filter) {
        System.out.println("in create Filter component widget");
        AudioComponent filterCW = new AudioComponent.Filters(50);
        AudioComponentWidget acw = new FilterWidget(filterCW, mainCanvas_, "Filter");
        existWidgets_.add(acw);
    }

    private void createMixer(String mixer) {
        System.out.println("in create Mixer component widget");
        AudioComponent mixerCW = new AudioComponent.Mixers();
        AudioComponentWidget acw = new MixerWidget(mixerCW, mainCanvas_, "Mixer");
        existWidgets_.add(acw);
    }


    public static void main(String[] args) {
        launch();
    }

    private AnchorPane mainCanvas_;
    public static Circle speaker_;

    private int volumeValue_ = 50;

    public static ArrayList<AudioComponentWidget> widgets_ = new ArrayList<>();

    public static ArrayList<AudioComponentWidget> existWidgets_ = new ArrayList<>();
}