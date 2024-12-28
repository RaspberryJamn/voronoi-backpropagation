#ifndef IDKFUNCTIONS_H
#define IDKFUNCTIONS_H

//template <typename T> void G_Clamp(T* value, T min, T max);
template <typename T> void G_Clamp(T* value, T min, T max) {
    if ((*value) < min) {
        (*value) = min;
    }
    if ((*value) > max) {
        (*value) = max;
    }
}

#endif // IDKFUNCTIONS_H
