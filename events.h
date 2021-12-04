#pragma once

namespace king
{
    enum class EventType
    {
        kRandom,
        kTimed,
        kWeather,
        kDisaster,
        kTarget
    };

    enum class TargetEffects
    {
        kKing,
        kArmy,
        kCommander,
        kCharacter
    };

// TODO: First add area events, and timed events. There should be conditional events as well.
    class Events
    {
    public:
        void update();
        void generate_random_events();
        void generate_event(EventType event_type);
    private:
    };
}