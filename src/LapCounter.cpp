#include "LapCounter.h"

LapCounter::LapCounter() {
    this->Clear();
}
LapCounter::~LapCounter() {
    this->Clear();
}
void LapCounter::Clear() {

    for (size_t i = 0; i < LAP_COUNTER_BUCKETS; i++) {
        this->ms_numerators[i] = 0;
        this->lap_denominators[i] = 0;
    }
    this->running_ms = 0;
    this->running_laps = 0;
    this->write_head = 0;

    this->timer.Stop();
}

void LapCounter::Reset() {
    this->Clear();
    this->timer.Reset();
}

void LapCounter::Pause() {
    this->timer.Pause();
}

void LapCounter::Resume() {
    this->timer.Unpause();
}

void LapCounter::CallLap() {
    this->running_laps++;
    this->running_ms = this->timer.QueryTicks();
    if (this->running_ms < LAP_COUNTER_MIN_BUCKET_LENGTH) {
        return;
    }
    if (this->running_laps < LAP_COUNTER_MIN_SLOW_FILL_LEVEL) {
        return;
    }
    this->timer.Reset();
    this->ms_numerators[write_head] = this->running_ms;
    this->running_ms = 0;
    this->lap_denominators[write_head] = this->running_laps;
    this->running_laps = 0;
    write_head++;
    if (write_head == LAP_COUNTER_BUCKETS) {
        write_head = 0;
    }
}

double LapCounter::GetAverageLapTime() {
    int cumm_ms = this->running_ms;
    int cumm_laps = this->running_laps;
    for (size_t i = 0; i < LAP_COUNTER_BUCKETS; i++) {
        cumm_ms += this->ms_numerators[i];
        cumm_laps += this->lap_denominators[i];
    }
    if (cumm_laps == 0) {
        return 0.0;
    }
    return ((double)cumm_ms)/((double)cumm_laps);
}
