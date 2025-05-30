#ifndef PARAMETER_MANAGER_HPP
#define PARAMETER_MANAGER_HPP

#include <vector>
#include <string>
#include <cstdint>

struct AdjustableParam {
    std::string name;
    float* value;
    float min;
    float max;
    float step;
};

class ParameterManager {
private:
    std::vector<AdjustableParam> adjustableParams;
    int currentParamIndex = 0;
    int8_t lastBlueKnob = 0;
    bool paramAdjustMode = false;
    bool showingSelectedParam = false;
    uint64_t paramSelectionTime = 0;

public:
    ParameterManager();
    void initAdjustableParams();

    void toggleParamAdjustMode();
    
    void handleBlueKnob(uint32_t knob_data);
    
    void drawParamUI(unsigned char* mem_base);
    
    bool isParamAdjustMode() const { return paramAdjustMode; }
    bool isShowingSelectedParam() const { return showingSelectedParam; }
    const std::vector<AdjustableParam>& getAdjustableParams() const { return adjustableParams; }
    int getCurrentParamIndex() const { return currentParamIndex; }
    
    void setLastBlueKnob(int8_t value) { lastBlueKnob = value; }
};

#endif