// Delaunay.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "topology.h"
#include <iostream>
#include <chrono>

bool DRAW_DELAUNAY = false;
bool DRAW_VORONOI = false;
bool DRAW_MST = false;

void DrawEdge(Edge* e, sf::RenderWindow& window, const sf::Color& color)
{
	Edge* sym = e->Sym();
	Vert* destination = e->destination();
	Vert org = *(e->origin());
	Vert dest = *(e->destination());

	sf::VertexArray v(sf::Lines, 2);

	v[0].position = sf::Vector2f(org.x(), org.y());
	v[0].color = color;
	v[1].position = sf::Vector2f(dest.x(), dest.y());
	v[1].color = color;

	window.draw(v);
}

void DrawVoronoi(Edge* e, sf::RenderWindow& window)
{
	sf::VertexArray v(sf::Lines, 2);

	Vert* org = e[1].origin();
	Vert* dest = e[3].origin();

	v[0].position = sf::Vector2f(org->x(), org->y());
	v[0].color = sf::Color::Green;
	v[1].position = sf::Vector2f(dest->x(), dest->y());
	v[1].color = sf::Color::Green;

	window.draw(v);
}

void Render(QuadList& quads, EdgeList& mst)
{
	// Build the remdering environment
	sf::RenderWindow window(sf::VideoMode(512, 512), "Delaunay Triangulator");

	// Turn the vertices into something we want to render
	std::vector<std::tuple<sf::Vector2f, sf::Vector2f>> quads_debug;
	for (int i = 0; i < quads.size(); i++)
	{
		sf::Vector2f a(quads[i]->edges[0].origin()->x(), quads[i]->edges[0].origin()->y());
		sf::Vector2f b(quads[i]->edges[2].origin()->x(), quads[i]->edges[2].origin()->y());
		quads_debug.push_back(std::make_tuple(a, b));
		//quads_debug.push_back(std::make_tuple(quads[i]->edges[1].origin()->getPosition(), quads[i]->edges[3].origin()->getPosition()));
	}

	// Rendering loop
	while (window.isOpen())
	{
		window.clear();

		for (auto i = quads.begin(); i != quads.end(); ++i)
		{
			if (DRAW_DELAUNAY && (*i)->edges[0].draw)
			{
				DrawEdge((*i)->edges, window, sf::Color::White);
			}
			if (DRAW_VORONOI && (*i)->edges[1].draw)
			{
				DrawVoronoi((*i)->edges, window);
			}
		}

		for (auto i = mst.begin(); i != mst.end(); ++i)
		{
			if (DRAW_MST && (*i)->draw)
			{
				DrawEdge((*i), window, sf::Color::Red);
			}
		}

		window.display();
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	int n;
	char yn;

	// Get number of vertices to triangulate
	std::cout << "Enter number of vertices to triangulate:" << std::endl;
	std::cin >> n;

	std::cout << "Draw Delaunay triangulation?" << std::endl;
	std::cin >> yn;
	if (yn == 'y' || yn == 'Y')
	{
		DRAW_DELAUNAY = true;
	}

	std::cout << "Draw Voronoi diagram?" << std::endl;
	std::cin >> yn;
	if (yn == 'y' || yn == 'Y')
	{
		DRAW_VORONOI = true;
	}
	std::cout << "Draw the spanning tree?" << std::endl;
	std::cin >> yn;
	if (yn == 'y' || yn == 'Y')
	{
		DRAW_MST = true;
	}

	auto t1 = std::chrono::high_resolution_clock::now();
	Delaunay del(n);
	del.GetTriangulation();
	QuadList quads = del.GetVoronoi();
	EdgeList mst = del.GetMST();
	auto t2 = std::chrono::high_resolution_clock::now();

	std::cout << "Running time (ms): " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << std::endl;

	Render(quads, mst);

	return 0;
}

