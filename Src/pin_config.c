#include "pin_config.h"


void init_pins() {

    LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

    LL_GPIO_ResetOutputPin(CTL1_GPIO_Port, CTL1_Pin);
    LL_GPIO_ResetOutputPin(CTL2_GPIO_Port, CTL2_Pin);
    LL_GPIO_ResetOutputPin(LED2_GPIO_Port, LED2_Pin);

    GPIO_InitStruct.Pin = SW1_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(SW1_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = CTL1_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(CTL1_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = CTL2_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(CTL2_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LED2_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(LED2_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = STAT_STBY_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(STAT_STBY_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = V_MAIN_Pin | V_STBY_Pin;
    GPIO_InitStruct.OutputType = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(S_PORT_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = S_PORT_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
    LL_GPIO_Init(S_PORT_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MAGNET_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(MAGNET_GPIO_Port, &GPIO_InitStruct);

    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE0);
    EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_0;
    EXTI_InitStruct.LineCommand = ENABLE;
    EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
    EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
    LL_EXTI_Init(&EXTI_InitStruct);
    NVIC_SetPriority(EXTI0_1_IRQn, 0);
    NVIC_EnableIRQ(EXTI0_1_IRQn);

#ifndef DEBUG_SWD_ENABLED
    // Configure SWD pins if not in debug mode
    GPIO_InitStruct.Pin = LED1_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);


    GPIO_InitStruct.Pin = SW2_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(SW2_GPIO_Port, &GPIO_InitStruct);

    // Set interrupt for SW2 pin
    // NOTE: interrupt does not work if programming clip is installed
    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE14);
    EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_14;
    EXTI_InitStruct.LineCommand = ENABLE;
    EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
    EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
    LL_EXTI_Init(&EXTI_InitStruct);
    NVIC_SetPriority(EXTI4_15_IRQn, 0);
    NVIC_EnableIRQ(EXTI4_15_IRQn);

#endif // DEBUG_SWD_ENABLED
}

void set_pin_to_analog(GPIO_TypeDef *gpio, uint32_t pin) {
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(gpio, &GPIO_InitStruct);    
}

void set_pin_to_output_PP_no_pull(GPIO_TypeDef *gpio, uint32_t pin) {
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(gpio, &GPIO_InitStruct);
}

void set_stop_mode_not_needed_gpio_pins_to_analog() {
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;

    GPIO_InitStruct.Pin = SW1_Pin;
    LL_GPIO_Init(SW1_GPIO_Port, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = LED2_Pin;
    LL_GPIO_Init(LED2_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = STAT_STBY_Pin;
    LL_GPIO_Init(STAT_STBY_GPIO_Port, &GPIO_InitStruct);

#ifndef DEBUG_SWD_ENABLED
    GPIO_InitStruct.Pin = LED1_Pin;
    LL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);         
#endif
}
