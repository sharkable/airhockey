
extern "C" {
  void init();
  void update();
  void touch(int action, double x, double y);
}


#include <GLES/gl.h>

#include "airhockey/views/rink_view.h"
#include "airhockey/views/splash_view.h"
#include "gameengine/android/ad_engine_android.h"
#include "gameengine/android/analytics_engine_android.h"
#include "gameengine/opengl/texture2d.h"
#include "gameengine/game_engine.h"
#include "gameengine/game_engine.h"

static sp<GameEngine> game_engine_;

void init() {
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  glDepthMask(false);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  // TODO This shouldn't be hardcoded. Rethink this anyways.
  Texture2D::SetScreenHeight(960);

  game_engine_.reset(new GameEngine());

  game_engine_->SetScreenSize(screen_size_make(640, 960), game_size_make(768, 1024));

  game_engine_->load_positions("assets/positions/game_menu_iphone.xml");
  game_engine_->load_positions("assets/positions/main_menu_iphone.xml");
  game_engine_->load_positions("assets/positions/play_iphone.xml");
  game_engine_->load_positions("assets/positions/rink_iphone.xml");

  sp<AdEngine> ad_engine = sp<AdEngine>(new AdEngineAndroid());
  game_engine_->set_ad_engine(ad_engine);

  sp<AnalyticsEngine> analytics_engine = sp<AnalyticsEngine>(new AnalyticsEngineAndroid());
  game_engine_->set_analytics_engine(analytics_engine);

  sp<EngineView> root_view = sp<EngineView>(new RinkView(game_engine_));
  game_engine_->SetRootView(root_view);

  sp<EngineView> splash_view = sp<EngineView>(new SplashView(game_engine_));
  game_engine_->PushView(splash_view);
}

#include "gameengine/opengl/Texture2D.h"

void update() {
  game_engine_->Update();

    int backingWidth_ = 640;
    int backingHeight_ = 960;

    glViewport(0, 0, backingWidth_, backingHeight_);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(0, backingWidth_, 0, backingHeight_, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

  game_engine_->Render();
}

#include "gameengine/touch.h"
#include <vector>
using std::vector;

void touch(int action, double x, double y) {
  vector<Touch> touches;
  Touch t;
  t.set_location(game_engine_->screen_point_to_game_point(screen_point_make(x, y)));
  t.set_identifier(0);
  touches.push_back(t);
  if (action == 0) {
    s_log("SET TOUCHES BEGAN");
    game_engine_->set_touches_began(touches);
  } else if (action == 1) {
    game_engine_->set_touches_ended(touches);
  } else if (action == 2) {
    game_engine_->set_touches_moved(touches);
  }
}
