#include "../JuceLibraryCode/JuceHeader.h"

class MainContentComponent   : public Component, public Button::Listener
{
public:
    //==============================================================================
    MainContentComponent()
    {
        if(! oscSender.connect(ip, portnumber)) std::cout<<"OSC connection Error..."<<std::endl;
        
        midiOut = MidiOutput::createNewDevice("JUCE MIDI");
        midiOut->startBackgroundThread();
        
        addAndMakeVisible(infoMidiOsc);
        infoMidiOsc.setText("MIDI\n PortName: JUCE MIDI,  Channel: 1,  Note: 60,  Velocity: 127\n\n OSC\n IP:127.0.0.1,  Port: 8080,  Address: /juce/osc, Value: (int) 1", NotificationType::dontSendNotification);
        
        addAndMakeVisible(midiButton);
        addAndMakeVisible(oscButton);
        midiButton.setButtonText("send MIDI");
        oscButton.setButtonText("send OSC");
        midiButton.addListener(this);
        oscButton.addListener(this);
        setSize (400, 200);
    }

    ~MainContentComponent()
    {
        midiOut->stopBackgroundThread();
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    }

    void resized() override
    {
        auto localBounds = getLocalBounds();
        auto infoLabelBounds = localBounds.removeFromTop(100);
        infoMidiOsc.setBounds(infoLabelBounds);
        midiButton.setBounds(localBounds.removeFromLeft(localBounds.getWidth() / 2));
        oscButton.setBounds(localBounds);
    }
    
    //==============================================================================
    void buttonClicked (Button* button) override
    {
        if (button == &midiButton)
        {
            sendMIDI();
        }
        else if(button == &oscButton)
        {
            sendOSC();
        }
    }
    
    //==============================================================================
    void sendOSC()
    {
        oscSender.send(oscAddress, oscValue);
    }
    
    void sendMIDI()
    {
        if (midiMessage.isNoteOn())
        {
            midiMessage = MidiMessage::noteOff(midiChannel, midiMessage.getNoteNumber(), (uint8)0);
            midiOut->sendMessageNow(midiMessage);
        }
        
        midiMessage = MidiMessage::noteOn(midiChannel, midiNote, (uint8)127);
        midiOut->sendMessageNow(midiMessage);
    }

private:
    //==============================================================================
    
    //GUI
    Label infoMidiOsc;
    TextButton midiButton;
    TextButton oscButton;
    
    //OSC
    OSCSender oscSender;
    const String ip = "127.0.0.1";
    const int portnumber = 8080;
    const String oscAddress = "/juce/osc";
    const int oscValue = 1;
    
    //MIDI
    ScopedPointer<MidiOutput> midiOut;
    MidiMessage midiMessage;
    const int midiNote = 60;
    const int midiChannel = 1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


Component* createMainContentComponent()     { return new MainContentComponent(); }
