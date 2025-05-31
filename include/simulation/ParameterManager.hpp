#ifndef PARAMETER_MANAGER_HPP
#define PARAMETER_MANAGER_HPP

#include <vector>
#include <string>
#include <cstdint>

/**
 * @brief Struct representing a tunable simulation parameter
 * @param value Pointer to the actual parameter value
 * @param min Minimum allowed value for the parameter
 * @param max Maximum allowed value for the parameter   
 * @param step Increment/decrement step for the parameter
 */
struct AdjustableParam {
    std::string name; 
    float* value;      
    float min;       
    float max;      
    float step;        
};

/**
 * @brief Manages runtime-adjustable parameters via knob input
 *
 * This class allows the user to:
 * - Toggle parameter adjustment mode
 * - Cycle through adjustable parameters
 * - Modify values using a blue knob
 * - Display the currently selected parameter and its value on screen
 */
class ParameterManager {
    /*
    * List of parameters to control
    * Currently selected parameter index
    * Last state of blue knob
    * Flag for adjustment mode
    * Flag to switch between selecting/adjusting
    * Time of last parameter change (for UI update)
    */
private:
    std::vector<AdjustableParam> adjustableParams; 
    int currentParamIndex = 0;                     
    int8_t lastBlueKnob = 0;                       
    bool paramAdjustMode = false;                  
    bool showingSelectedParam = false;             
    uint64_t paramSelectionTime = 0;      
public:
    /**
     * @brief Construct a new Parameter Manager object
     */
    ParameterManager();

    /**
     * @brief Initializes the list of adjustable parameters
     */
    void initAdjustableParams();

    /**
     * @brief Toggles parameter adjustment mode on or off
     */
    void toggleParamAdjustMode();

    /**
     * @brief Handles input from the blue knob to adjust parameters
     *
     * @param knob_data Raw knob register input
     */
    void handleBlueKnob(uint32_t knob_data);

    /**
     * @brief Draws the parameter UI and updates LEDs to reflect the current value
     *
     * @param mem_base Pointer to mapped memory
     */
    void drawParamUI(unsigned char* mem_base);

    /**
     * @return True if in parameter adjustment mode
     */
    bool isParamAdjustMode() const { return paramAdjustMode; }

    /**
     * @return True if in parameter selection state
     */
    bool isShowingSelectedParam() const { return showingSelectedParam; }

    /**
     * @return List of used parameters
     */
    const std::vector<AdjustableParam>& getAdjustableParams() const { return adjustableParams; }

    /**
     * @return Index of the currently selected parameter
     */
    int getCurrentParamIndex() const { return currentParamIndex; }

    /**
     * @brief Sets the last blue knob value
     */
    void setLastBlueKnob(int8_t value) { lastBlueKnob = value; }
};

#endif 
