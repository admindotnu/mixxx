import Mixxx 0.1 as Mixxx
import QtQuick 2.12

Item {
    id: root

    property alias focusWidget: focusedWidgetControl.value

    signal moveVertical(int offset)
    signal loadSelectedTrack(string group, bool play)
    signal loadSelectedTrackIntoNextAvailableDeck(bool play)

    FocusedWidgetControl {
        id: focusedWidgetControl

        Component.onCompleted: this.value = FocusedWidgetControl.WidgetKind.LibraryView
    }

    Mixxx.ControlProxy {
        group: "[Library]"
        key: "GoToItem"
        onValueChanged: {
            if (value != 0 && root.focusWidget == FocusedWidgetControl.WidgetKind.LibraryView)
                root.loadSelectedTrackIntoNextAvailableDeck(false);

        }
    }

    Mixxx.ControlProxy {
        group: "[Playlist]"
        key: "LoadSelectedIntoFirstStopped"
        onValueChanged: {
            if (value != 0 && root.focusWidget == FocusedWidgetControl.WidgetKind.LibraryView)
                root.loadSelectedTrackIntoNextAvailableDeck(false);

        }
    }

    Mixxx.ControlProxy {
        group: "[Playlist]"
        key: "SelectTrackKnob"
        onValueChanged: {
            if (value != 0) {
                root.focusWidget = FocusedWidgetControl.WidgetKind.LibraryView;
                root.moveVertical(value);
            }
        }
    }

    Mixxx.ControlProxy {
        group: "[Playlist]"
        key: "SelectPrevTrack"
        onValueChanged: {
            if (value != 0) {
                root.focusWidget = FocusedWidgetControl.WidgetKind.LibraryView;
                root.moveVertical(-1);
            }
        }
    }

    Mixxx.ControlProxy {
        group: "[Playlist]"
        key: "SelectNextTrack"
        onValueChanged: {
            if (value != 0) {
                root.focusWidget = FocusedWidgetControl.WidgetKind.LibraryView;
                root.moveVertical(1);
            }
        }
    }

    Mixxx.ControlProxy {
        group: "[Library]"
        key: "MoveVertical"
        onValueChanged: {
            if (value != 0 && root.focusWidget == FocusedWidgetControl.WidgetKind.LibraryView)
                root.moveVertical(value);

        }
    }

    Mixxx.ControlProxy {
        group: "[Library]"
        key: "MoveUp"
        onValueChanged: {
            if (value != 0 && root.focusWidget == FocusedWidgetControl.WidgetKind.LibraryView)
                root.moveVertical(-1);

        }
    }

    Mixxx.ControlProxy {
        group: "[Library]"
        key: "MoveDown"
        onValueChanged: {
            if (value != 0 && root.focusWidget == FocusedWidgetControl.WidgetKind.LibraryView)
                root.moveVertical(1);

        }
    }

    Mixxx.ControlProxy {
        id: numDecksControl

        group: "[Master]"
        key: "num_decks"
    }

    Instantiator {
        model: numDecksControl.value

        delegate: LibraryControlLoadSelectedTrackHandler {
            group: "[Channel" + (index + 1) + "]"
            enabled: root.focusWidget == FocusedWidgetControl.WidgetKind.LibraryView
            onLoadTrackRequested: root.loadSelectedTrack(group, play)
        }

    }

    Mixxx.ControlProxy {
        id: numPreviewDecksControl

        group: "[Master]"
        key: "num_preview_decks"
    }

    Instantiator {
        model: numPreviewDecksControl.value

        delegate: LibraryControlLoadSelectedTrackHandler {
            group: "[PreviewDeck" + (index + 1) + "]"
            enabled: root.focusWidget == FocusedWidgetControl.WidgetKind.LibraryView
            onLoadTrackRequested: root.loadSelectedTrack(group, play)
        }

    }

    Mixxx.ControlProxy {
        id: numSamplersControl

        group: "[Master]"
        key: "num_samplers"
    }

    Instantiator {
        model: numSamplersControl.value

        delegate: LibraryControlLoadSelectedTrackHandler {
            group: "[Sampler" + (index + 1) + "]"
            enabled: root.focusWidget == FocusedWidgetControl.WidgetKind.LibraryView
            onLoadTrackRequested: root.loadSelectedTrack(group, play)
        }

    }

}
