void controllerEmergencyHandler() {
    if(emergency_status == EMERGENCY_HANDLED) {
        switch (state) {
            case STATE_UP:
                emergency_status = EMERGENCY_ABOVE;
                break;
            case STATE_DOWN:
                emergency_status = EMERGENCY_BELOW;
                break;
            case STATE_STARTUP:
                emergency_status = EMERGENCY_BELOW;
            default:
                break;
            }
    }
    state = STATE_EMERGENCY_STOP;
}