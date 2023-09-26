package com.example.synthesizer;

import javafx.geometry.Bounds;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Pane;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Line;

import static com.example.synthesizer.SynthesizeApplication.existWidgets_;
import static com.example.synthesizer.SynthesizeApplication.widgets_;

public class AudioComponentWidget extends Pane {

    AudioComponentWidget(AudioComponent ac, AnchorPane parent, String name) {
        audioComponent_ = ac;
        parent_ = parent;
        name_ = name;
        widgetIamSendingOutputTo_ = null;

        widgetIamConnected_ = null;


        // Base Layout
        baseLayout = new HBox();
        baseLayout.setStyle("-fx-border-color: black; -fx-border-image-width: 8; -fx-background-color: white");

        // Left Side
        leftSide = new VBox();
        inputCircle = new Circle(10);// Input Circle
        inputCircle.setFill(Color.BLUE);

        leftSide.getChildren().add(inputCircle);
        leftSide.setAlignment(Pos.CENTER);
        leftSide.setPadding(new Insets(5));
        leftSide.setSpacing(5);

        // Right Side
        rightSide = new VBox();
        Button closeBtn = new Button("x");
        closeBtn.setOnAction(e -> closeWidget());
        Circle output = new Circle(10);
        output.setFill(Color.RED);
        output.setOnMousePressed(e -> startConnection(e, output));
        output.setOnMouseDragged(e -> moveConnection(e, output));
        output.setOnMouseReleased(e -> endConnection(e));


        Button removeBtn = new Button("R");
        removeBtn.setOnAction(e -> removeLine());

        rightSide.getChildren().add(closeBtn);
        rightSide.getChildren().add(output);
        rightSide.getChildren().add(removeBtn);
        rightSide.setAlignment(Pos.CENTER);
        rightSide.setPadding(new Insets(5));
        rightSide.setSpacing(5);

        // Center Portion of Widget
        center = new VBox();
        center.setAlignment(Pos.CENTER_LEFT);
        center.setPadding(new Insets(5));
        center.setSpacing(5);

        nameLabel_ = new Label();
        nameLabel_.setMouseTransparent(true);
        center.getChildren().add(nameLabel_);

        center.setOnMousePressed(e -> startDrag(e));
        center.setOnMouseDragged(e -> handleDrag(e, output));


        baseLayout.getChildren().add(leftSide);
        baseLayout.getChildren().add(center);
        baseLayout.getChildren().add(rightSide);
        this.getChildren().add(baseLayout);

        parent_.getChildren().add(this);

    }

    private void removeLine() {
        if (line_ != null) {
            // remove that line so than we can create a new connection..
            parent_.getChildren().remove(line_);
            widgets_.remove(this);
            line_ = null;
            if (widgetIamSendingOutputTo_ != null) {
                System.out.println("remove line");
                widgetIamSendingOutputTo_.audioComponent_.removeInput(this.getAudioComponent());
                widgetIamSendingOutputTo_ = null;
            }
        }
    }


    private void startConnection(MouseEvent e, Circle output) {

        // If a line exists ( in other words, if we are connected to someone else)
        if (line_ != null) {
            // remove that line so than we can create a new connection..
            parent_.getChildren().remove(line_);
            widgets_.remove(this);
        }

        Bounds parentBounds = parent_.getBoundsInParent();
        Bounds bounds = output.localToScene(output.getBoundsInLocal());

        line_ = new Line();
        line_.setStrokeWidth(4);
        line_.setStartX(bounds.getCenterX() - parentBounds.getMinX());
        line_.setStartY(bounds.getCenterY() - parentBounds.getMinY());
        line_.setEndX(e.getSceneX());
        line_.setEndY(e.getSceneY());

        // For any widget (ie, this 'line_') we have to add it tot the parent
        // so that it will be drawn by javafx
        parent_.getChildren().add(line_);
    }

    private void moveConnection(MouseEvent e, Circle output) {
        Bounds parentBounds = parent_.getBoundsInParent();
        line_.setEndX(e.getSceneX() - parentBounds.getMinX());
        line_.setEndY(e.getSceneY() - parentBounds.getMinY());
    }

    private void endConnection(MouseEvent e) {

        // line to speaker
        Circle speaker = SynthesizeApplication.speaker_;
        Bounds speakerBounds = speaker.localToScreen(speaker.getBoundsInLocal());
        double distance = Math.sqrt(Math.pow(speakerBounds.getCenterX() - e.getScreenX(), 2.0) +
                Math.pow(speakerBounds.getCenterY() - e.getScreenY(), 2.0));

        if (distance < 10) {
            // Handle actually connecting to speaker

            widgets_.add(this);
        } else {
            if (existWidgets_.size() > 1) {

                // line to input
                for (int i = 0; i < existWidgets_.size(); i++) {
                    Circle input = existWidgets_.get(i).inputCircle;
                    Bounds inputBounds = input.localToScreen(input.getBoundsInLocal());

                    if (inputBounds != null) {
                        double distanceToInput = Math.sqrt(Math.pow(inputBounds.getCenterX() - e.getScreenX(), 2.0) +
                                Math.pow(inputBounds.getCenterY() - e.getScreenY(), 2.0));

                        if (distanceToInput < 10) {
                            // Handle actually connecting to input
                        widgetIamSendingOutputTo_ = existWidgets_.get(i);
                        widgetIamSendingOutputTo_.setAudioConnected(this);
                        }
                    }
                }
            } else {
                parent_.getChildren().remove(line_);
                line_ = null;
                widgets_.remove(this);
            }
        }
    }


    private void startDrag(MouseEvent e) {
        mouseStartDragX_ = e.getSceneX();
        mouseStartDragY_ = e.getSceneY();

        widgetStartDragX_ = this.getLayoutX();
        widgetStartDragY_ = this.getLayoutY();
    }

    private void handleDrag(MouseEvent e, Circle outputCircle) {
        double mouseDelX = e.getSceneX() - mouseStartDragX_;
        double mouseDelY = e.getSceneY() - mouseStartDragY_;
        this.relocate(widgetStartDragX_ + mouseDelX, widgetStartDragY_ + mouseDelY);

        if (line_ != null) {
            Bounds parentBounds = parent_.getBoundsInParent();
            Bounds bounds = outputCircle.localToScene(outputCircle.getBoundsInLocal());
            line_.setStartX(bounds.getCenterX() - parentBounds.getMinX());
            line_.setStartY(bounds.getCenterY() - parentBounds.getMinY());
        }
        if (widgetIamConnected_ != null && widgetIamConnected_.line_ != null) {
            Bounds parentBounds = parent_.getBoundsInParent();
            Bounds bounds = inputCircle.localToScene(outputCircle.getBoundsInLocal());
            widgetIamConnected_.line_.setEndX(bounds.getCenterX() - parentBounds.getMinX());
            widgetIamConnected_.line_.setEndY(bounds.getCenterY() - parentBounds.getMinY());
        }
    }

    private void closeWidget() {
        parent_.getChildren().remove(this);
        widgets_.remove(this);
        parent_.getChildren().remove(line_);
        line_ = null;
        existWidgets_.remove(this);
        if (widgetIamConnected_ != null) {
            parent_.getChildren().remove(widgetIamConnected_.line_);
            widgetIamConnected_.line_ = null;
        }
    }

    public AudioComponent getAudioComponent() {
        return audioComponent_;
    }

    public void setAudioConnected(AudioComponentWidget connected) {
        widgetIamConnected_ = connected;
    }

    public AudioComponent audioComponent_;
    private AudioComponentWidget widgetIamSendingOutputTo_;
    public AudioComponentWidget widgetIamConnected_;
    private AnchorPane parent_;
    private String name_;
    public HBox baseLayout;
    public VBox leftSide;
    public VBox center;
    public VBox rightSide;
    public Circle inputCircle;
    private Line line_;
    public Label nameLabel_;
    double mouseStartDragX_, mouseStartDragY_, widgetStartDragX_, widgetStartDragY_;
}
