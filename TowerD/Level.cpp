#include "stdafx.h"
#include "Level.h"

Level::Level(int tileSize, sf::Vector2i screenSize, Renderer* ren)
	:mTileSize(tileSize), mScreenSize(screenSize), mRen(ren)
{
	enemy = new Enemy(sf::Vector2f(600, 200), mTileSize, mRen);
	LoadLevel(1);
	mDirections = { sf::Vector2i(1, 0), sf::Vector2i(-1, 0), sf::Vector2i(0, 1), sf::Vector2i(0, -1), sf::Vector2i(1, 1), sf::Vector2i(-1, -1), sf::Vector2i(-1, 1), sf::Vector2i(1, -1) };
	mCam = Camera(mPlayers[0]->AimPos(), mPlayers[0]->Pos());
}

void Level::LoadLevel(int level)
{
	for each (std::vector<Tile*> row in mTiles)
	{
		for each (Tile* t in row)
		{
			delete(t);
		}
	}
	mTiles.clear();
	ifstream myReadFile;
	string line;
	myReadFile.open("../Levels/Level" + std::to_string(level) + ".txt");
	if (myReadFile.is_open())
	{
		int y = 0;
		while (getline(myReadFile, line))
		{
			if (line == "-")
				break;
			mMapSize.x = line.size();
			mTiles.push_back(std::vector<Tile*>());
			for (int x = 0; x < line.length(); x++)
			{
				string s = "";
				int type = atoi((s + line.at(x)).c_str());
				if (type == 2)
				{
					mPlayerSpawn = sf::Vector2i(x * 32, y * 32);
					mPlayers.push_back(new Player(sf::Vector2f(mPlayerSpawn), mTileSize, mRen));
					mPlayers.back()->LoadAssets();
					mCores.push_back(new Core(sf::Vector2f(mPlayerSpawn), mTileSize, mRen));
					mCores.back()->LoadAssets();
					type = 0;
				}
				else if (type == 3)
				{
					mEnemySpawns.push_back(sf::Vector2i(x * 32, y * 32));
					type = 0;
				}
				mTiles[y].push_back(new Tile(sf::Vector2i(x * 32, y * 32), mTileSize, type, mRen));
				mTiles.at(y).at(x)->LoadAssets();
			}
			y++;
		}
		mMapSize.y = y;
		mWaves.push_back(std::map<int, std::pair<int, int>>());
		while (getline(myReadFile, line))
		{
			if (line == "-")
				mWaves.push_back(std::map<int, std::pair<int, int>>());
			int wave[3];
			for (int i = 0, start = 0, end = line.find(',', start + 1); i < 3; start = end, i++)
				wave[i] = atoi((line.substr(start, end - start)).c_str());
			mWaves.back()[wave[0]] = std::pair<int, int>(wave[1], wave[2]);
		}
	}
	myReadFile.close();
	mNodes = new Node *[mMapSize.x * mMapSize.y];
	int n = 0;
	for (int i = 0; i < mTiles.size(); i++)
		for (int i2 = 0; i2 < mTiles[0].size(); i2++, n++)
			mNodes[n] = new Node(sf::Vector2i(i2,i));
	enemy->LoadAssets();
}

void Level::Update(float t, sf::Vector2i mPos)
{
	for each (Player* p in mPlayers)	
	{
		p->Update(t, mCam.Offset(), mCam.Scale());
		std::pair<int, sf::Vector2i> toPlace = p->GetPlace();
		if (toPlace.first == 0)
		{
			Tile* t = mTiles[toPlace.second.y / mTileSize][toPlace.second.x / mTileSize];
			if (t->Type() == 0)
			{
				delete(t);
				mTiles[toPlace.second.y / mTileSize][toPlace.second.x / mTileSize] = new Tile(toPlace.second, mTileSize, 2, mRen);
				mTiles[toPlace.second.y / mTileSize][toPlace.second.x / mTileSize]->LoadAssets();
				//CalcArrowPath();
			}
		}
	}
	for each (Core* c in mCores)
		c->Update(t);
	mCam.Update(mPos);
	enemy->Update(t, mCores[0]->Location());
}


//calculates the paths from each enemy spawn to the core
//also takes the position of a possible newly placed tile
//and returns whether or not a path was found
bool Level::CalcArrowPath(sf::Vector2i newPos)
{
	std::vector<std::vector<Sprite*>> tempArrows;
	std::vector<std::vector<std::pair<sf::Vector2i, float>>> tempArrowPaths;
	for each (sf::Vector2i start in mEnemySpawns)
	{
		start /= 32;
		sf::Vector2i goal = sf::Vector2i(mCores[0]->Location() / 32.f);
		std::vector<std::vector<Tile*>> mTempTiles = mTiles;
		mTempTiles[newPos.y][newPos.x] = new Tile(newPos * mTileSize, mTileSize, 2, mRen);
		Node* goalNode = mNodes[goal.y*mMapSize.x + goal.x];
		//modified Dijkstras algorithm
		priority_queue<Node*, vector<Node*>, Compare> pq;
		for (int i = 0; i < mMapSize.x*mMapSize.y; i++) {
			mNodes[i]->Distance = INT_MAX;
			mNodes[i]->Previous = 0;
			mNodes[i]->Marked = false;
			mNodes[i]->Visited = false;
		}
		Node* startNode = mNodes[start.y*mMapSize.x + start.x];
		startNode->Distance = 0;
		startNode->Marked = true;
		pq.push(startNode);
		Node* top = pq.top();
		while (pq.size() > 0 && pq.top() != goalNode)
		{
			int n = 0;
			top = pq.top();
			pq.pop();
			top->Visited = true;
			for (int i = 1; i < mDirections.size(); i++)
			{
				sf::Vector2i dir = mDirections[i];
				//makes sure potential tiles are within maps bounds and are of type floor
				if (top->Position.x + dir.x > 0 && top->Position.x + dir.x < mMapSize.x && top->Position.y + dir.y > 0 && top->Position.y + dir.y < mMapSize.y
					&& mTempTiles[top->Position.y + dir.y][top->Position.x + dir.x]->Type() == 0)
				{
					//makes sure path doesnt go in between diagonal walls and doesnt cut corners
					//makes use of the order it checks the directions
					if (n <= 3 || (mTempTiles[top->Position.y][top->Position.x + dir.x]->Type() == 0 && mTempTiles[top->Position.y + dir.y][top->Position.x]->Type() == 0))
					{
						Node* c = mNodes[(top->Position.y + dir.y)*mMapSize.x + top->Position.x + dir.x];
						if (!c->Visited)
						{
							int distC = (n > 3 ? 7 : 5) + top->Distance;
							if (distC < c->Distance)
							{
								c->Distance = distC;
								c->Previous = top;
								if (!c->Marked)
								{
									c->Marked = true;
									pq.push(c);
								}
								else //ensures queue has been sorted correctly
									make_heap(const_cast<Node**>(&pq.top()), const_cast<Node**>(&pq.top()) + pq.size(), Compare());
							}
						}
					}
				}
				n++;
			}
		}
		if (pq.size() == 0 && top != goalNode)
			return false;
		//creates paths from the results
		std::vector<std::pair<sf::Vector2i, float>> path;
		std::vector<std::pair<sf::Vector2i, float>>::iterator it = path.begin();
		path.insert(it, std::pair<sf::Vector2i, float>(goalNode->Position, 0));
		for (Node* temp = goalNode->Previous; temp != 0; temp = temp->Previous)
		{
			it = path.begin();
			std::pair < sf::Vector2i, float > thing(temp->Position, atan2(it->first.y - temp->Position.y, it->first.x - temp->Position.x));
			path.insert(it, thing);
		}
		//creates the arrow sprites
		tempArrowPaths.push_back(path);
		std::vector<Sprite*> arrows;
		for (int i = 0; i < path.size() - 1; i++)
		{
			Sprite* s = new Sprite("../Sprites/Arrow.png");
			s->setOrigin(s->getTextureRect().width / 2, s->getTextureRect().height / 2);
			s->setRotation(path[i].second / 3.14159 * 180);
			arrows.push_back(s);
			mRen->Add(s, TILE);
		}
		tempArrows.push_back(arrows);
	}
	//cleans up old arrows
	mArrowPaths.clear();
	for each (std::vector<Sprite*> v in mArrows)
	{
		for each (Sprite* a in v)
		{
			mRen->Remove(a, 12);
		}
		ClearVector(&v);
	}
	mArrows.clear();
	mArrowPaths = tempArrowPaths;
	mArrows = tempArrows;

	return true;
}

//update scale and offset of arrows, tiles and entities and then draws
void Level::Draw(sf::RenderWindow* win)
{
	for (int i = 0; i < mArrows.size(); i++)
	{
		for (int i2 = 0; i2 < mArrows[i].size(); i2++)
		{
			mArrows[i][i2]->setPosition((sf::Vector2f(mArrowPaths[i][i2].first * 32) - mCam.Offset())*mCam.Scale());
			mArrows[i][i2]->setScale(mCam.Scale()*.5f, mCam.Scale()*.5f);
		}
	}
	for (int x = 0; x < mTiles.size(); x++)
		for (int y = 0; y < mTiles[0].size(); y++)
			mTiles[x][y]->Draw(mCam.Offset(), mCam.Scale());
	for each (Entity* e in mEntities)
		e->Draw(mCam.Offset(), mCam.Scale());
	mRen->Draw(win);
}

void Level::CheckCollision()
{
	for each (Player* p in mPlayers)
	{
		sf::Vector2i index = sf::Vector2i((p->Location().x + mTileSize / 2) / mTileSize, (p->Location().y + mTileSize / 2) / mTileSize);
		for each (sf::Vector2i t in mDirections)
		{
			if ((p->Location().x - (mTileSize + p->Size().x) / 2 < (index.x + t.x) * mTileSize &&
				p->Location().x + (mTileSize + p->Size().x) / 2 > (index.x + t.x) * mTileSize) &&
				(p->Location().y - (mTileSize + p->Size().y) / 2 < (index.y + t.y) * mTileSize &&
				p->Location().y + (mTileSize + p->Size().y) / 2 > (index.y + t.y) * mTileSize))
				if (mTiles[index.y + t.y][index.x + t.x]->Type() == 2 || mTiles[index.y + t.y][index.x + t.x]->Type() == 1)
					p->Collision(mTiles[index.y + t.y][index.x + t.x]->Rect());
		}
	}
	sf::Vector2i index = sf::Vector2i((enemy->Location().x + mTileSize / 2) / mTileSize, (enemy->Location().y + mTileSize / 2) / mTileSize);
	for each (sf::Vector2i t in mDirections)
	{
		if ((enemy->Location().x - (mTileSize + enemy->Size().x) / 2 < (index.x + t.x) * mTileSize &&
			enemy->Location().x + (mTileSize + enemy->Size().x) / 2 > (index.x + t.x) * mTileSize) &&
			(enemy->Location().y - (mTileSize + enemy->Size().y) / 2 < (index.y + t.y) * mTileSize &&
			enemy->Location().y + (mTileSize + enemy->Size().y) / 2 > (index.y + t.y) * mTileSize))
			if (mTiles[index.y + t.y][index.x + t.x]->Type() == 2 || mTiles[index.y + t.y][index.x + t.x]->Type() == 1)
				enemy->Collision(mTiles[index.y + t.y][index.x + t.x]->Rect());
	}
}

void Level::ProcessInput(sf::Event e)
{
	for each (Player* p in mPlayers)
	{
		p->ProcessInput(e, mCam.Offset(), mCam.Scale());
	}
}