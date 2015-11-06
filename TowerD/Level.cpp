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


void Level::CalcArrowPath()
{
	mArrowPaths.clear();
	for each (sf::Vector2i start in mEnemySpawns)
	{
		start /= 32;
		sf::Vector2i goal = sf::Vector2i(mCores[0]->Location() / 32.f);
		Node* goalNode = mNodes[goal.y*mMapSize.x + goal.x];
		priority_queue<Node*, vector<Node*>, Compare> pq;
		list<Node*> previousNodes;
		for (int i = 0; i < mMapSize.x*mMapSize.y; i++) {
			mNodes[i]->Distance = INT_MAX;
			mNodes[i]->Previous = 0;
		}
		Node* startNode = mNodes[start.y*mMapSize.x+start.x];
		startNode->Distance = 0;
		startNode->Marked = true;
		pq.push(startNode);
		while (pq.size() > 0 && pq.top() != goalNode)
		{
			previousNodes.push_back(pq.top());
			int n = 0;
			for each (sf::Vector2i dir in mDirections)
			{
				if (pq.top()->Position.x + dir.x > 0 && pq.top()->Position.x + dir.x < mMapSize.x && pq.top()->Position.y + dir.y > 0 && pq.top()->Position.y + dir.y < mMapSize.y && mTiles[pq.top()->Position.y + dir.y][pq.top()->Position.x + dir.x]->Type() == 0)
				{
					Node* c = mNodes[(pq.top()->Position.y + dir.y)*mMapSize.x + pq.top()->Position.x + dir.x];
					if (!(std::find(previousNodes.begin(), previousNodes.end(), c) != previousNodes.end()))
					{
						int distC = n > 3 ? 7 : 5 + pq.top()->Distance;
						if (distC < c->Distance)
						{
							c->Distance = distC;
							c->Previous = pq.top();
						}
						if (!c->Marked)
						{
							pq.push(c);
							c->Marked = true;
						}
					}

				}
				n++;
			}
			pq.pop();
		}
		mArrowPaths.push_back(std::vector<std::pair<sf::Vector2i, float>>());
		std::vector<std::pair<sf::Vector2i, float>> path = mArrowPaths.back();
		std::vector<std::pair<sf::Vector2i, float>>::iterator it = path.begin();
		path.insert(it, std::pair<sf::Vector2i, float>(goalNode->Position, 0));
		for (Node* temp = goalNode->Previous; temp != 0; temp = temp->Previous)
		{
			it = path.begin();
			std::pair < sf::Vector2i, float > thing(temp->Position, atan2(temp->Position.y - it->first.y, temp->Position.x - it->first.x));
			path.insert(it, thing);
		}
	}
}

void Level::Draw(sf::RenderWindow* win)
{
	for (int x = 0; x < mTiles.size(); x++)
		for (int y = 0; y < mTiles[0].size(); y++)
			mTiles[x][y]->Draw(mCam.Offset(), mCam.Scale());
	for each (Player* p in mPlayers)
		p->Draw(mCam.Offset(), mCam.Scale());
	for each (Core* c in mCores)
		c->Draw(mCam.Offset(), mCam.Scale());
	enemy->Draw(mCam.Offset(), mCam.Scale());
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