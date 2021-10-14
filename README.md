<h1 align="center" style="border-bottom: none;">
	<a href="https://github.com/hexoctal/zenith">Zenith</a>
</h1>
<h3 align="center">A light and fast cross-platform 2D game framework written in C++ using SDL2.</h3>
<p align="center">
	<a href="#features">Features</a> |
	<a href="#sample-code">Sample Code</a> |
	<a href="#plans">Plans</a> |
	<a href="#dependencies">Dependencies</a>
</br>
</br>
<a href=""><img alt="platforms" src="https://img.shields.io/static/v1?label=Platforms&message=Windows%20|%20Linux&color=blue&style=flat-square"></a>
<a href="https://github.com/hexoctal/zenith/blob/master/LICENSE.md"><img alt="license" src="https://img.shields.io/static/v1?label=License&message=MIT&color=green&style=flat-square"></a>
</br>
<a href="https://github.com/hexoctal/zenith/issues"><img alt="issues" src="https://img.shields.io/github/issues-raw/hexoctal/zenith.svg?style=flat-square"/></a>
<a href=""><img alt="size" src="https://img.shields.io/github/repo-size/hexoctal/zenith?style=flat-square"></a>
<a href=""><img alt="starts" src="https://img.shields.io/github/stars/hexoctal/zenith?style=social"></a>
</br>
</p>

#

Zenith is a lightweight cross-platform 2D game framework, for making games and other multimedia applications targeted at desktop computers. Its interface and overall structure is heavily inspired by [Phaser 3](https://phaser.io/).

## Features

* Lightweight and extra fast
* Support for Linux and Windows (I don't have MacOS to test but it might work there too)
* 3D audio using OpenAL (3D sound in a 2D framework, crazy amaright?)
* Easy to use interface
* Entity Component System (ECS) using ENTT
* OpenGL / Shaders / Render Pipeline System

## Sample Code

```cpp
#include "zenith.hpp"

class TestScene : public Zen::Scene {
public:
	Zen::Entity ball;

	TestScene () : Scene("test")
	{}

	void preload ()
	{
		load.image("ball", "assets/ball.png");
	}

	void create ()
	{
		ball = add.image(400, 300, "ball");
	}

	void update (Uint32 time, Uint32 delta)
	{
		Zen::SetX(ball, Zen::GetX(ball) + 1);
	}

};

int main ()
{
	Zen::GameConfig config;

	config.setBackgroundColor(0xffff00)
		.setWidth(800)
		.setHeight(600)
		.addScenes<TestScene>();

	Zen::Game game (config);
}
```

## Plans

* Animation system
* Add 2D physics with Box2D
* Asynchronous resource loader

## Dependencies

* [SDL2](https://www.libsdl.org/) : Cross-platform multimedia library
* [OpenGL](https://www.opengl.org) : Cross-platform API for rendering 2D and 3D vector graphics
* [GLM](https://github.com/g-truc/glm) : header only C++ mathematics library for graphics software based on the GLSL specifications
* [OpenAL Soft](https://github.com/kcat/openal-soft) : 3D Audio API
* [entt](https://github.com/skypjack/entt) : A fast and reliable entity component system (ECS)
* [JSON for Modern C++](https://github.com/nlohmann/json) : A C++ library for JSON support
* [FreeType 2](https://www.freetype.org/) : A freely available software library to render fonts
* [Earcut](https://github.com/mapbox/earcut.hpp) : Fast, header-only polygon triangulation
