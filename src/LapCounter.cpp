#include "LapCounter.h"

LapCounter::LapCounter() {
    this->laps = 0;
    this->timer.Stop();
}

LapCounter::~LapCounter() {

}

void LapCounter::Reset() {
    this->timer.Reset();
    this->laps = 0;
}

void LapCounter::Stop() {
    this->timer.Stop();
    this->laps = 0;
}

void LapCounter::AddLaps(int laps) {
    this->laps += laps;
}

double LapCounter::GetAverageLapTime() {
    if (this->laps == 0) {
        return 0.0;
    }
    return (this->timer.QueryTicks())/((double)this->laps);
}
