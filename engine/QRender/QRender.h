#pragma once
#ifndef QRENDER_H
#define QRENDER_H

#include "IRenderable.h"
#include "RenderManager.h"

QRENDER_PUBLIC RenderManager* InitRender(RenderEngines render_engine = RenderEngines::SYSTEM_SELECTION);

#endif