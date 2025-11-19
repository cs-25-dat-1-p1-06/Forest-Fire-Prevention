typedef enum {NORTH,EAST,SOUTH,WEST} wind_e;
struct Wind{
    wind_e direction;
    double speed;
};
void start_wind(struct Wind* wind);
void update_wind(struct Wind* wind);
void print_wind(struct Wind* wind);