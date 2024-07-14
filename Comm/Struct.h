// comm_structs.h
#ifndef COMM_STRUCTS_H
#define COMM_STRUCTS_H

struct CommMsg {
    double x;
    double y;
    double z;
    double a;
    double b;
    double c;
    int ackn;
};

struct Offset {
    double x_off;
    double y_off;
    double z_off;
    double a_rot;
    double b_rot;
    double c_rot;
    int enter;
};

struct Limit {
    double x_neg;
    double x_poz;
    double y_neg;
    double y_poz;
    double z;
};

struct XYZ {
    float x, y, z;
};

#endif // COMM_STRUCTS_H
