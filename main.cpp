#include "main.h"
#include <stdio.h>



//const uint8_t BUZZER = 26; 
void IRAM_ATTR onTimer(){
	ledcWrite(2, 0);
}

void IRAM_ATTR Timer3_ISR()
{
    tm_rdy++;
}

void setup() {
   
  // put your setup code here, to run once:
  Serial.begin(9600);
  mutex  = xSemaphoreCreateMutex();
  Prefs.begin("myPrefs", false);
  QueueHandle_t commandQueue;

  uart_init();
  // Настройка таймера для канала 0
 ledc_timer_config_t ledc_timer_0 = {
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .duty_resolution = LEDC_TIMER_11_BIT,
    .timer_num = LEDC_TIMER_0,
    .freq_hz = 16000,
    .clk_cfg = LEDC_AUTO_CLK,
};
ledc_timer_config(&ledc_timer_0);
  ledc_channel_0.channel = LEDC_CHANNEL_0;
  ledc_channel_0.duty = 0;
  ledc_channel_0.gpio_num = STEP_PIN;
  ledc_channel_0.speed_mode = LEDC_HIGH_SPEED_MODE;
  ledc_channel_0.hpoint = 0;
  ledc_channel_0.timer_sel = LEDC_TIMER_0;
  ledc_channel_config(&ledc_channel_0);

  lv_init();
  tft.begin();       /* TFT init */
	
  ledcSetup(2,987, 8); //2349
  ledcAttachPin(26,2);
  
  My_timer = timerBegin(2, 80, true);
  timerAttachInterrupt(My_timer, &onTimer, true);
  Buzzer(100);
  
  digitalWrite(TFT_BL, LOW);
  tft.setRotation(1);//( 3 ); /* Landscape orientation, flipped */
  tft.fillScreen(ILI9341_WHITE);
  
 // touch.setCal(526, 3443, 750, 3377, 320, 240, 1);              
  touch.setCal(550, 3500, 750, 3377, 320, 240, 1);  	
  lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 40 );

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init( &disp_drv );
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush; //刷新
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register( &disp_drv );

  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init( &indev_drv );
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register( &indev_drv );

  
  ui_init();
//   read_settings(LAST_SETTINGS);
   
  Timer3_Cfg = timerBegin(3, 80, true);
  timerAttachInterrupt(Timer3_Cfg, &Timer3_ISR, true);
  timerAlarmWrite(Timer3_Cfg, 1000000, true);
  timerAlarmEnable(Timer3_Cfg);


  attachInterrupt(36, ISR, FALLING);
  pinMode(27,INPUT);
  digitalWrite(27,HIGH);
  ledcSetup(0,1000, 8);
  ledcAttachPin(TFT_BL,0);
  ledcWrite(0, 255);
    //xTaskCreatePinnedToCore(executeTask, "ExecuteTask", 4096*4, mutex,1, NULL, 1);
    xTaskCreatePinnedToCore(LVGLTimer, "Task1", 4096*4, mutex, 1, NULL, 0);
    xTaskCreatePinnedToCore(task_main, "Task2", 4096*4, mutex, 1, NULL, 1);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void LVGLTimer(void *pvParameters){
   xSemaphoreTake(mutex, portMAX_DELAY);
    lv_scr_load_anim(ui_Screen1,LV_SCR_LOAD_ANIM_MOVE_LEFT,0,2000,0);
    xSemaphoreGive(mutex);
  for(;;){
    if (pdTRUE == xSemaphoreTake(mutex, portMAX_DELAY)) {
      lv_timer_handler_run_in_period(5); 
      xSemaphoreGive(mutex);
    }
   vTaskDelay(5/ portTICK_RATE_MS);
  }
}

void task_main(void *pvParameters){
    //Serial.println("Main task started.");
    setupProgram();
    init_values();
    vTaskDelay(200 / portTICK_RATE_MS);
    //strcpy(tx_buff, "Ki100\r\n");
    //uart_write_bytes(uart_num, (const char*)tx_buff, strlen(tx_buff));
    //Serial.println("Initial values set.");

    for(;;) { 

        Parameters *param = (Parameters *) pvParameters;
        if(startflag){  
            switch (tab1) {
                case 0:
                    //Serial.println("Starting centrifuge task.");
                    taskFunc(centrifuge, ui_LblTimCen, ui_BtnPlaCen);
                    startflag = 0;
                    break;
                case 1:
                    //Serial.println("Starting vortex task.");
                    taskFunc(vortex, ui_LblTimVor, ui_BtnPlaVor);
                    startflag = 0;
                    break;
                case 2:
                    //Serial.println("Starting execute command task.");
                    executeCommand();
                    startflag = 0;
                    break;
                default:
                    //Serial.println("Invalid task selection.");
                    break;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));    
    }
}

void taskFunc(Parameters& taskParams, lv_obj_t* ui_LblTim, lv_obj_t* ui_BtnPla) {
    //Serial.println("Task started.");
    if(stopflag) {
        current.timeset = taskParams.timeset;
        stopflag = 0;
        //Serial.println("Task stop flag reset.");
    }
    for(;;) { 
        if(digitalRead(27))
          {
            stopflag=1;
            Buzzer(100);
          }
        if(tm_rdy) {
            //Serial.println("Task timer ready.");
            if(!pauseflag && taskParams.timeset>0) {
                xSemaphoreTake(mutex, portMAX_DELAY);
                updateTimeLabel(ui_LblTim, current.timeset);
                xSemaphoreGive(mutex);
                current.timeset--;
                //Serial.printf("Time set: %d seconds remaining.\n", current.timeset);
                if(current.timeset == 0) { 
                     xSemaphoreTake(mutex, portMAX_DELAY);
                    _ui_flag_modify(ui_Hider, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                    _ui_state_modify(ui_BtnPla, LV_STATE_CHECKED, _UI_MODIFY_STATE_REMOVE);
                    xSemaphoreGive(mutex);
                    stopflag = 1;
                    Buzzer(1500);  
                   // Serial.println("Task completed.");
                } else {
                    Motor_control(taskParams.mode, taskParams.value);
                    _ui_flag_modify(ui_Hider, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                    _ui_flag_modify(ui_Hider1, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);                    
                }
            } else if (!pauseflag && !taskParams.timeset) {
                //Serial.println("Time set is zero. Running motor control.");
                Motor_control(taskParams.mode, taskParams.value);
            } else { 
                //Serial.println("Task paused or stopped.");
                Motor_control(2, 0);
                _ui_flag_modify(ui_Hider, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_Hider1, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);                  
            }
            tm_rdy = 0;
        }
        if(stopflag) {
            lv_obj_clear_state(ui_BtnPla, LV_STATE_CHECKED);
            init_values();
            Motor_control(2, 0);
            _ui_flag_modify(ui_Hider, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
            _ui_flag_modify(ui_Hider1, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
            //Serial.println("Task stopped.");
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(10));   
    }
}

void executeCommand() {
    //Serial.println("Execute command task started.");
    if(stopflag) {
        current.timeset = program[0].timeset;
        current.value = program[0].value;
        stopflag = 0;
        tab2 =0;
        lv_tabview_set_act(ui_TabView2, 0, LV_ANIM_OFF);
        //Serial.println("Task stop flag reset.");
    }    
    while(tab2 < 4) {
        if(tm_rdy) {
            //Serial.printf("Processing program step %d\n", tab2);
            if(program[tab2].mode < 3) {
                current.mode = program[tab2].mode;
                current.value = program[tab2].value;
                current.timeset = program[tab2].timeset;
                //Serial.printf("Mode: %d, Value: %d, Timeset: %d\n", current.mode, current.value, current.timeset);
                while(current.timeset > 0) {
                    if(tm_rdy&& !pauseflag) {
                        tm_rdy = 0;
                        current.timeset--;
                        Motor_control(current.mode, current.value);
                        sprintf(SprBuff, "%02d:%02d", current.timeset / 60, current.timeset % 60);
                        lv_label_set_text(ui_LblTim[tab2], SprBuff);
                        //Serial.printf("Remaining time: %d seconds\n", current.timeset);
                    }else if (pauseflag)
                    {
                        Motor_control(2, 0);
                        _ui_flag_modify(ui_Hider, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                        _ui_flag_modify(ui_Hider1, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                    }
                    
                    vTaskDelay(pdMS_TO_TICKS(10));
                }
                tab2 += 1;
                TabwiewUpdate();
                Buzzer(500);
                //Serial.println("Buzzer activated for 500 ms.");
                    vTaskDelay(pdMS_TO_TICKS(10));
            } else if(program[tab2].mode == 3) {
                //Serial.println("Mode 3 detected.");
                if(endofcycle) {
                    _ui_state_modify(ui_BtnPla1, LV_STATE_CHECKED, _UI_MODIFY_STATE_REMOVE);
                    current.mode = program[tab2].mode; 
                    current.repeat = program[tab2].value;
                    //Serial.println("Stop command flag is set. Exiting.");
                    endofcycle=0; 
                    TabwiewUpdate();
                    vTaskDelay(pdMS_TO_TICKS(10));
                }

                if(current.repeat) {
                    current.repeat--;
                    sprintf(SprBuff, "%02d", current.repeat);
                    lv_label_set_text(ui_LblVal[tab2], SprBuff);
                    tab2 = 0;
                    current.mode = program[0].mode;
                    current.value = program[0].value;
                    current.timeset = program[0].timeset;
                    TabwiewUpdate();
                    Buzzer(500);
                    //Serial.printf("Repeating command. Tab2 to 0.%d repeats left\n", current.repeat);
                }else{
                tab2 += 1;
                TabwiewUpdate();
                Buzzer(500);
                //Serial.printf("Repeating command. Tab2 to 0.%d repeats left\n", current.repeat);
                }
                vTaskDelay(pdMS_TO_TICKS(10));

            } else if(program[tab2].mode == 4) {
                //Serial.println("Mode 4 detected . Stopping execution.");
                stopflag = 1;
                startflag = 0;
                _ui_state_modify(ui_BtnPla1, LV_STATE_CHECKED, _UI_MODIFY_STATE_REMOVE);
                Motor_control(2, 0);
                Buzzer(2000);
                tab2 = 0;
                lv_tabview_set_act(ui_TabView2, 0, LV_ANIM_OFF);
                TabwiewUpdate();
                vTaskDelay(pdMS_TO_TICKS(10));
                //Serial.println("Task completed. Motor stopped.");
                endofcycle=1;
                break;
            }
        }
        tm_rdy = 0; 
    }
                stopflag = 1;
                startflag = 0;
                Motor_control(2, 0);
                tab2 = 0;
                init_values();
                TabwiewUpdate();
                _ui_state_modify(ui_BtnPla1, LV_STATE_CHECKED, _UI_MODIFY_STATE_REMOVE);
                endofcycle=1; 
                //Serial.println("Task completed. Motor stopped.");
}

void TabwiewUpdate()
{
    xSemaphoreTake(mutex, portMAX_DELAY);
    lv_tabview_set_act(ui_TabView2, tab2, LV_ANIM_OFF);
    //Serial.printf("Moving to program step %d\n", tab2);
    switch (program[tab2].mode)
    {
    case 2:
        _ui_flag_modify(ui_PanTim[tab2], LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        _ui_flag_modify(ui_PanlVal[tab2], LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
        //Serial.println("Mode 2: Timer panel shown, Value panel hidden.");
        break;
    case 3:
        _ui_flag_modify(ui_PanlVal[tab2], LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        _ui_flag_modify(ui_PanTim[tab2], LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
        //Serial.println("Mode 3: Value panel shown, Timer panel hidden.");
        break;
    case 4:
        _ui_flag_modify(ui_PanlVal[tab2], LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
        _ui_flag_modify(ui_PanTim[tab2], LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
        //Serial.println("Mode 4: Both panels hidden.");
        break;
    default:
        _ui_flag_modify(ui_PanlVal[tab2], LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        _ui_flag_modify(ui_PanTim[tab2], LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        //Serial.println("Default: Showing both ui_PanlVal1 and ui_PanTim.");
        break;
    }
    sprintf(SprBuff, "%3d", program[tab2].value);
    lv_label_set_text(ui_LblVal[0], SprBuff);
    lv_dropdown_set_selected(ui_DrpDwn[tab2], program[tab2].mode);
    xSemaphoreGive(mutex);
}

void Motor_control(int mode, int value){

  if((prevmode != mode)||(prevvalue != value))
  {
    switch (mode)
    {
    case 0:
      Serial.printf("Spin%d\r\n",value);
      break;
    case 1:
      Serial.printf("Shake %d\r\n",(1+value)*50);
      break;
    case 2:
      Serial.print("Stop\r\n");
      break;
    case 3:
      Serial.print("Brake\r\n");
      break;      
    default:
      Serial.println("Invalid Value\r\n");
      break;
    }
    prevvalue  = value;
    prevmode  = mode;
  }  

}

void setupProgram() {
    centrifuge = {110, 500, 0, 300, 6000, 50}; 
    vortex = {130, 3, 1, 1, 5, 1};
    program[0] = {10, 1000, 0}; // spin
    program[1] = {10, 3, 1};    // shake
    program[2] = {10, 0, 2};    // wait
    program[3] = {0, 2, 3};     // loop
    program[4] = {0, 0, 4};     // terminate
    stopflag = 1;
    tabprev = 0;
    //Serial.println("Program setup completed.");
}

void init_values(){
    
    xSemaphoreTake(mutex, portMAX_DELAY);
    lv_tabview_set_act(ui_TabView2,tab2, LV_ANIM_OFF);
    
    sprintf(SprBuff, "%02d:%02d", centrifuge.timeset / 60, centrifuge.timeset % 60);
    lv_label_set_text(ui_LblTimCen, SprBuff);
    
    sprintf(SprBuff, "%02d:%02d", vortex.timeset / 60, vortex.timeset % 60);
    lv_label_set_text(ui_LblTimVor, SprBuff);
    
    sprintf(SprBuff, "%3d", centrifuge.value);
    lv_label_set_text(ui_LblValCen, SprBuff);
    
    sprintf(SprBuff, "%3d", vortex.value);
    lv_label_set_text(ui_LblValVor, SprBuff);
    
    for (int tab2 = 0; tab2 < 5; tab2++)
    {
        lv_dropdown_set_selected(ui_DrpDwn[tab2], program[tab2].mode); 
        sprintf(SprBuff, "%3d", program[tab2].value);
        lv_label_set_text(ui_LblVal[tab2], SprBuff);
        sprintf(SprBuff, "%02d:%02d", program[tab2].timeset / 60, program[tab2].timeset % 60);
        lv_label_set_text(ui_LblTim[tab2], SprBuff);
        switch (program[tab2].mode)
        {
        case 2:
            _ui_flag_modify(ui_PanTim[tab2], LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
            _ui_flag_modify(ui_PanlVal[tab2], LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
            lv_dropdown_set_selected(ui_DrpDwn[tab2],program[tab2].mode);
            break;
        case 3:
            _ui_flag_modify(ui_PanlVal[tab2], LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
            _ui_flag_modify(ui_PanTim[tab2], LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
            lv_dropdown_set_selected(ui_DrpDwn[tab2],program[tab2].mode);
            break;
        case 4:
            _ui_flag_modify(ui_PanlVal[tab2], LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
            _ui_flag_modify(ui_PanTim[tab2], LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
            lv_dropdown_set_selected(ui_DrpDwn[tab2],program[tab2].mode);
            break;
        default:
            _ui_flag_modify(ui_PanlVal[tab2], LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
            _ui_flag_modify(ui_PanTim[tab2], LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
            lv_dropdown_set_selected(ui_DrpDwn[tab2],program[tab2].mode);
            break;
        }

    }    
    xSemaphoreGive(mutex);
    //Serial.println("Initial values set in UI.");
}

 void uart_init(){
   uart_config_t uart_config = {
      .baud_rate = 9600,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
      .source_clk = UART_SCLK_APB,
  };
  uart_driver_install(uart_num, 1024 * 2, 1024 * 2, 0, NULL, 0);
  uart_param_config(uart_num, &uart_config);// Настройка параметров UART
  uart_set_pin(UART_NUM_0, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
 }

void IRAM_ATTR ISR(){
  touchpad_pressed = 1;
  detachInterrupt(36);
}

void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();

    lv_disp_flush_ready( disp );
}

void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
    uint16_t touchX, touchY;
	uint16_t rawtouchX, rawtouchY; 
    //bool touched = touch.Pressed();//tft.getTouch( &touchX, &touchY, 600 );
    //Serial.println(touch.ReadRawY());
    if(touchpad_pressed==1&&digitalRead(36)==LOW){  
      touch.Pressed();
      /*Set the coordinates*/
      data->point.x = touchX = touch.X();
      data->point.y = touchY = touch.Y();
      data->state = LV_INDEV_STATE_PR;
    }
    else
    {   
      data->state = LV_INDEV_STATE_REL;
      touchpad_pressed = 0;
      attachInterrupt(36, ISR, FALLING);
    }
}

void Buzzer(uint16_t ms){
	ledcWrite(2, 6);//6
	timerRestart(My_timer);
	timerAlarmWrite(My_timer, ms*1000, false);
    timerAlarmEnable(My_timer);
}

