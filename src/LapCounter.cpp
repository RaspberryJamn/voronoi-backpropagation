#include "LapCounter.h"

LapCounter::LapCounter() {
    this->ms_numerators[0] = 0;
    this->ms_numerators[1] = 0;
    this->ms_numerators[2] = 0;
    this->ms_numerators[3] = 0;
    this->ms_numerators[4] = 0;
    this->lap_denominators[0] = 0;
    this->lap_denominators[1] = 0;
    this->lap_denominators[2] = 0;
    this->lap_denominators[3] = 0;
    this->lap_denominators[4] = 0;
    this->running_ms = 0;
    this->running_laps = 0;
    this->write_head = 0;

    this->timer.Stop();
}

LapCounter::~LapCounter() {
    this->ms_numerators[0] = 0;
    this->ms_numerators[1] = 0;
    this->ms_numerators[2] = 0;
    this->ms_numerators[3] = 0;
    this->ms_numerators[4] = 0;
    this->lap_denominators[0] = 0;
    this->lap_denominators[1] = 0;
    this->lap_denominators[2] = 0;
    this->lap_denominators[3] = 0;
    this->lap_denominators[4] = 0;
    this->running_ms = 0;
    this->running_laps = 0;
    this->write_head = 0;

    this->timer.Stop();
}

void LapCounter::Reset() {
    this->timer.Reset();
}

void LapCounter::CallLap() {
    this->running_laps++;
    this->running_ms = this->timer.QueryTicks();
    if (this->running_ms < 10) {
        return;
    }
    if (this->running_laps < 3) {
        return;
    }
    this->timer.Reset();
    this->ms_numerators[write_head] = this->running_ms;
    this->running_ms = 0;
    this->lap_denominators[write_head] = this->running_laps;
    this->running_laps = 0;
    write_head++;
    if (write_head >= 5) {
        write_head = 0;
    }
}

double LapCounter::GetAverageLapTime() {
    int cumm_ms = this->running_ms;
    int cumm_laps = this->running_laps;
    for (size_t i = 0; i < 5; i++) {
        cumm_ms += this->ms_numerators[i];
        cumm_laps += this->lap_denominators[i];
    }
    if (cumm_laps == 0) {
        return 0.0;
    }
    return ((double)cumm_ms)/cumm_laps;
}
