struct Object {
    unsigned int unknown_00;
    unsigned int field_04;
    int present();
};
int Object::present() { return field_04 != 0; }
