#include <pebble.h>
#include <stdlib.h>
#include <stdio.h>

char *strtokp(s, delim)
register char *s;
register const char *delim;
{
  register char *spanp;
  register int c, sc;
  char *tok;
  static char *last;


  if (s == NULL && (s = last) == NULL)
  return (NULL);
  cont:
  c = *s++;
  for (spanp = (char *)delim; (sc = *spanp++) != 0;) {
    if (c == sc)
    goto cont;
  }
  if (c == 0) {
    last = NULL;
    return (NULL);
  }
  tok = s - 1;
  for (;;) {
    c = *s++;
    spanp = (char *)delim;
    do {
      if ((sc = *spanp++) == c) {
        if (c == 0)
        s = NULL;
        else
        s[-1] = 0;
        last = s;
        return (tok);
      }
    } while (sc != 0);
  }
}

static Window *s_wind;
static TextLayer *s_time;
static ScrollLayer *s_scroll;
static TextLayer *s_text;
static char* words[1000];
int numwords = 0;
int curword=0;
static int posns[1000];
static GRect bounds;
time_t initial;
static char content[4000];

void twodigit(int num, char* buffer) {
  for(int i = 0; i < 2; i++) {
    buffer[1-i] = '0' + (num % 10);
    num /= 10;
  }
}

void refactor(){
  APP_LOG(APP_LOG_LEVEL_INFO, "Test");
  char* point = strtokp(content, " \n");
  APP_LOG(APP_LOG_LEVEL_INFO, point);
  while(point != NULL){
    snprintf(words[numwords], 30, "%s ", point);
    numwords++;
    point = strtokp(NULL, " \n");
  }
  int pos, i, j, k;
  i = j = k = pos = 0;
  while(i < numwords){
    int len = strlen(words[i]);
    if(k+len < 20){
      posns[i]=j;
      snprintf(content+pos, 30, "%s ", words[i]);
      APP_LOG(APP_LOG_LEVEL_INFO, "%s, %i,  %i", words[i], i, j);
      pos+=len+1;
      k += len;
      i++;
    } else{
      snprintf(content+pos-1, 30, "\n");
      k=0; j++;
    }
  }
}

/*void combine(char* s, int pos){
  while(strlen(content) > 0){
    words[numwords] = strtok(s, " ");
    numwords++;
  }
}*/

static void moveto(int pos){
  GPoint offset = scroll_layer_get_content_offset(s_scroll);
  pos = (pos < 0) ? 0 : pos;
  offset.y = -pos*18-4;
  scroll_layer_set_content_offset(s_scroll, offset, false);
}

static void update_time() {
  time_t now = time(NULL);

  int diff = (5*60) + difftime(initial, now);
  diff = diff >= 0 ? diff : 0;

  static char s_buffer[8];
  snprintf(s_buffer, 8, "%i:", diff / 60);
  int len = strlen(s_buffer);
  twodigit(diff % 60, s_buffer+len);
  s_buffer[len+2] = '\0';
  text_layer_set_text(s_time, s_buffer);

  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  // Add a key-value pair
  dict_write_uint8(iter, 0, 0);

  // Send the message!
  app_message_outbox_send();

  if(diff % 15 == 0){
    //vibes_double_pulse();
  }
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  bounds = layer_get_bounds(window_layer);

  s_time = text_layer_create(GRect(0, 139, bounds.size.w, 30));

  text_layer_set_background_color(s_time, GColorBlack);
  text_layer_set_text_color(s_time, GColorWhite);
  text_layer_set_text(s_time, "5:00");
  text_layer_set_font(s_time, fonts_get_system_font(FONT_KEY_LECO_28_LIGHT_NUMBERS));
  text_layer_set_text_alignment(s_time, GTextAlignmentCenter);

  layer_add_child(window_layer, text_layer_get_layer(s_time));

  s_scroll = scroll_layer_create(GRect(0, 0, bounds.size.w, 142));
  scroll_layer_set_shadow_hidden(s_scroll, true);
  layer_add_child(window_layer, scroll_layer_get_layer(s_scroll));

  s_text = text_layer_create(GRect(bounds.origin.x, bounds.origin.y, bounds.size.w, 10000));
  snprintf(content, 100, "This\nis\na");
  text_layer_set_text(s_text, content);
  text_layer_set_text_alignment(s_text, GTextAlignmentCenter);
  text_layer_set_font(s_text, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  scroll_layer_add_child(s_scroll, text_layer_get_layer(s_text));

  GSize text_size = text_layer_get_content_size(s_text);

  text_layer_set_size(s_text, GSize(bounds.size.w, text_size.h));

  scroll_layer_set_content_size(s_scroll, text_size);

  GPoint offset = scroll_layer_get_content_offset(s_scroll);
  offset.y -= 19;
  scroll_layer_set_content_offset(s_scroll, offset, false);

  initial = time(NULL);
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time);
  scroll_layer_destroy(s_scroll);
  text_layer_destroy(s_text);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Entering func.");
  Tuple *datum = dict_find(iterator, MESSAGE_KEY_Script);
  Tuple *datum2 = dict_find(iterator, MESSAGE_KEY_Position);
  if(datum){
    text_layer_set_size(s_text, GSize(bounds.size.w, 10000));
    snprintf(content, 5000, datum->value->cstring);
    refactor();
    APP_LOG(APP_LOG_LEVEL_INFO, content);
    text_layer_set_text(s_text, content);
    GSize text_size = text_layer_get_content_size(s_text);
    text_layer_set_size(s_text, GSize(bounds.size.w, text_size.h));

    scroll_layer_set_content_size(s_scroll, text_size);

    GPoint offset = scroll_layer_get_content_offset(s_scroll);
    offset.y -= 4;
    scroll_layer_set_content_offset(s_scroll, offset, false);
  } else{
    curword = datum2->value->int32;
    APP_LOG(APP_LOG_LEVEL_INFO, "Current word: %i, line: %i", curword, posns[curword]);
    int line = posns[curword];
    moveto(line-1);
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void init() {
  s_wind = window_create();
  window_set_window_handlers(s_wind, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_wind, true);
  window_set_background_color(s_wind, GColorLavenderIndigo);
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  const int inbox_size = 5000;
  const int outbox_size = 400;
  app_message_open(inbox_size, outbox_size);
  for(int i=0; i<1000; i++){
    words[i] = malloc(30 * sizeof(char));
  }
}

static void deinit() {
  window_destroy(s_wind);
  for(int i=0; i<1000; i++){
    free(words[i]);
  }
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
