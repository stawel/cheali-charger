
void infiniteLoop() {
    while(1) {}
}

void _exit() {
    infiniteLoop();
}

void _sbrk() {
    infiniteLoop();
}
void _kill() {
    infiniteLoop();
}
void _getpid() {
    infiniteLoop();
}
