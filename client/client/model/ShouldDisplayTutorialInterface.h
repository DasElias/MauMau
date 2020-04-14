#pragma once

namespace card {
    enum class TutorialMessage {
        SKIP_CARD_EXPLANATION,
        SKIP_STATE_AND_CAN_PLAY,
        SKIP_STATE,
        DRAW_TWO_CARD_EXPLANATION,
        DRAW_TWO_STATE_AND_CAN_PLAY,
        DRAW_TWO_STATE,
        PLAY_CARD_STATE,
        DRAW_CARD_STATE,
        GAME_START,
        MAU_REMEMBER,
        TURN_END
    };

	class ShouldDisplayTutorialInterface {
        // ----------------------------------------------------------------------
        // -----------------------------CONSTRUCTORS-----------------------------
        // ----------------------------------------------------------------------
        public:
            ShouldDisplayTutorialInterface() = default;
            virtual ~ShouldDisplayTutorialInterface() = default;

        // ----------------------------------------------------------------------
        // -------------------------------METHODS--------------------------------
        // ----------------------------------------------------------------------
        public:
            virtual bool shouldDisplay(TutorialMessage msg) =0;
            virtual bool shouldDisplayAndClear(TutorialMessage msg) =0;
            virtual void clear(TutorialMessage msg) =0;

	};
}