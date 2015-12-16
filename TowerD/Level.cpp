#include "stdafx.h"
#include "Level.h"

Level::Level(int tileSize, sf::Vector2i screenSize, Renderer* ren)
	:mTileSize(tileSize), mScreenSize(screenSize), mRen(ren)
{
	mDirections = { sf::Vector2i(0, 0), sf::Vector2i(1, 0), sf::Vector2i(-1, 0), sf::Vector2i(0, 1), sf::Vector2i(0, -1), sf::Vector2i(1, 1), sf::Vector2i(-1, -1), sf::Vector2i(-1, 1), sf::Vector2i(1, -1) };
	//LoadLevel(1);
	Sprite::SetKeep("../Sprites/Arrow.png", true);
}

//Load map and wave info from file
void Level::LoadLevel(int level)
{
	for each (std::vector<Tile*> row in mTiles)
		ClearVector(&row);
	ClearVector(&mPlayers);
	ClearVector(&mCores);
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
					mPlayerSpawn = sf::Vector2i(x * mTileSize, y * mTileSize);
					AddEntity(new Core(sf::Vector2f(mPlayerSpawn), mTileSize, mRen));
					type = 0;
				}
				else if (type == 3)
				{
					mEnemySpawns.push_back(sf::Vector2i(x * mTileSize, y * mTileSize));
					type = 0;
				}
				mTiles[y].push_back(new Tile(sf::Vector2i(x * mTileSize, y * mTileSize), mTileSize, type, mRen));
				mTiles.at(y).at(x)->LoadAssets();
			}
			y++;
		}
		AddEntity(new Player(sf::Vector2f(mPlayerSpawn), mTileSize, mRen));
		mPlayerLocs.push_back(mPlayers.back()->Pos());
		mMapSize.y = y;
		//waves stored in as a vector containing vectors for each enemy spawn containing maps
		//with the spawn time as the key and a pair containing enemy type and count as the value
		mWaves.push_back(std::vector<std::map<int, std::pair<int, int>>>());
		for (int i = 0; i < mEnemySpawns.size(); i++)
			mWaves.back().push_back(std::map<int, std::pair<int, int>>());
		while (getline(myReadFile, line))
		{
			if (line == "-")
			{
				mWaves.push_back(std::vector<std::map<int, std::pair<int, int>>>());
				for (int i = 0; i < mEnemySpawns.size(); i++)
					mWaves.back().push_back(std::map<int, std::pair<int, int>>());
				getline(myReadFile, line);
			}
			if (line.substr(0, 2) != "//")
			{
				int wave[4];
				for (int i = 0, start = -1, end = line.find(',', start + 1); i < 4; start = end, end = line.find(',', start + 1), i++)
					wave[i] = atoi((line.substr(start + 1, end - start - 1)).c_str());
				int a = line.find(',', 2);
				mWaves.back()[wave[1]][wave[0]] = std::pair<int, int>(wave[3], wave[2]);
			}
		}
	}
	myReadFile.close();
	//nodes used in pathfinding
	mNodes = new Node *[mMapSize.x * mMapSize.y];
	int n = 0;
	for (int i = 0; i < mTiles.size(); i++)
		for (int i2 = 0; i2 < mTiles[0].size(); i2++, n++)
		{
		mNodes[n] = new Node(sf::Vector2i(i2, i));
		}
	CalcArrowPath(sf::Vector2i());
	mCam = Camera(mPlayers[0]->AimPos(), mPlayers[0]->Pos(), sf::Vector2f(mScreenSize));
}

void Level::Update(float t, sf::Vector2i mPos)
{
	t = 0.002f;
	//enemy spawning
	if (!mDefensePhase)
	{
		int sec = (int)mGameClock.getElapsedTime().asSeconds();
		if (sec != mSecond)
		{
			mSecond = sec;
			for (int i2 = 0; i2 < mEnemySpawns.size(); i2++)
			{
				//if map contains the current second enemies are queued to be spawned
				if (mWaves[mWave][i2].count(sec))
				{
					mSpawningEnemy = mWaves[mWave][i2][sec].second;
					mSpawnCount = mWaves[mWave][i2][sec].first;
					mSpawn = i2;
				}
			}
		}
		//no enemies left means wave is over
		if (mEnemiesLeft == 0)
			mDefensePhase = true;
	}
	//staggered enemy spawning 
	if (mSpawnCount > 0)
	{
		if ((int)mGameClock.getElapsedTime().asMilliseconds() % 200 == 0)
		{
			AddEntity(new Enemy(sf::Vector2f(mEnemySpawns[mSpawn]) + sf::Vector2f(8 - rand() % 16, 8 - rand() % 16), mTileSize, mRen, mPlayerLocs));
			mEnemies.back()->SetPath(mArrowPaths[mSpawn]);
			mSpawnCount--;
		}
	}

	for each (Player* p in mPlayers)
	{
		//checks if player wants to place a wall/turret and then does so
		if (mDefensePhase)
		{
			std::pair<int, sf::Vector2i> toPlace = p->GetPlace();
			if (toPlace.first == 1)
			{
				Tile* t = mTiles[toPlace.second.y / mTileSize][toPlace.second.x / mTileSize];
				if (t->Type() == 0)
				{
					if (CalcArrowPath(toPlace.second / mTileSize))
					{
						delete(t);
						mTiles[toPlace.second.y / mTileSize][toPlace.second.x / mTileSize] = new Tile(toPlace.second, mTileSize, 2, mRen);
						mTiles[toPlace.second.y / mTileSize][toPlace.second.x / mTileSize]->LoadAssets();
					}
				}
			}
			else if (toPlace.first == 2)
			{
				Tile* t = mTiles[toPlace.second.y / mTileSize][toPlace.second.x / mTileSize];
				if (t->Type() == 2 && !t->Occupied())
				{
					AddEntity(new Turret(sf::Vector2f(toPlace.second), mTileSize, mRen, &mEnemies));
					mTurrets.back()->LoadAssets();
					t->SetOccupied(true);
				}
			}
		}
		//if player is not alive starts the spawn timer and then spawns player
		if (!p->Alive())
		{
			if (!mPlayerSpawning){
				mRespawnTimer.restart();
				mPlayerSpawning = true;
			}
			if (mRespawnTimer.getElapsedTime().asSeconds() > 3){
				p->Respawn();
				mPlayerSpawning = false;
			}
		}
		//prevents player from shooting while ontop of a placed wall
		if (mTiles[((p->Location().y + mTileSize / 2) / mTileSize)][((p->Location().x + mTileSize / 2) / mTileSize)]->Type() != 2)
			p->Shoot();
	}

	for each (Turret* t in mTurrets)
	{
		t->Shoot();
	}

	for each (Enemy* e in mEnemies)
	{
		//finds the closest player that enemy can see
		float minDist = 1000000;
		sf::Vector2f player;
		for each (Player* p in mPlayers)
		{
			sf::Vector2i startPos = sf::Vector2i(e->Location() + sf::Vector2f(mTileSize / 2, mTileSize / 2));
			sf::Vector2i endPos = sf::Vector2i(p->Location() + sf::Vector2f(mTileSize / 2, mTileSize / 2));
			sf::Vector2i temp = startPos;
			if (startPos.x > endPos.x)
			{
				startPos = endPos;
				endPos = temp;
			}
			sf::Vector2f diff = p->Location() - e->Location();
			float dist = sqrt(diff.y * diff.y + diff.x * diff.x);
			bool line = true;
			//iterates along the x tiles from player to enemy
			for (int x = startPos.x / mTileSize; x <= endPos.x / mTileSize; x++)
			{
				//finds what y position the line from enemy to player will be at either side of tiles at x
				int yStart = max((int)((diff.y / diff.x) * (max(x * mTileSize, startPos.x) - startPos.x) + startPos.y), startPos.y) / mTileSize;
				int yEnd = max((int)((diff.y / diff.x) * (min((x + 1) * mTileSize, endPos.x) - endPos.x) + endPos.y), endPos.y) / mTileSize;
				int yTemp = yStart;
				if (yStart > yEnd)
				{
					yStart = yEnd;
					yEnd = yTemp;
				}
				//iterates down row of tiles the line crosses and checks for wall
				for (int y = yStart; y <= yEnd; y++)
				{
					int type = mTiles.at(y).at(x)->Type();
					if (type == 1 || type == 2)
					{
						line = false;
						break;
					}
				}
				if (!line)
					break;
			}
			//makes sure theres a line and its the closest player
			if (dist < minDist && line)
			{
				minDist = dist;
				player = p->Location();
			}
		}
		e->Shoot(player, minDist);
	}
	//update all entities and camera
	for each (Entity* e in mEntities)
		e->Update(t, mCam.Offset(), mCam.Scale());
	mCam.Update(mPos);
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

//all collisions are checked here
void Level::CheckCollision()
{
	//checks for player bullets hitting a wall
	for each (Player* p in mPlayers)
		for each (sf::Vector2f* h in p->GetGun()->GetBullets())
		{
		bool hit = false;
		sf::Vector2f halfTile = sf::Vector2f(mTileSize / 2.f, mTileSize / 2.f);
		sf::Vector2f hitPos = *h + halfTile;
		sf::Vector2f start = p->Location() + halfTile;
		if (mTiles[start.y / mTileSize][start.x / mTileSize]->Type() == 2)
		{
			hit = true;
			*h = start - halfTile;
			p->GetGun()->SetDrawRange(0);
			break;
		}
		sf::Vector2f dir = hitPos - start;
		dir /= sqrt(dir.x*dir.x + dir.y*dir.y);
		//every loop will find the next tile along the bullets path by finding the closest edge to the current position with the given direction
		while (!hit)
		{
			sf::Vector2f next = sf::Vector2f((int)(start.x + mTileSize * (dir.x / fabs(dir.x))) / mTileSize, (int)(start.y + mTileSize * (dir.y / fabs(dir.y))) / mTileSize) * (float)mTileSize + sf::Vector2f((dir.x < 0 ? mTileSize - 0.001f : 0), (dir.y < 0 ? mTileSize - 0.001f : 0));
			if (fabs((start.x - next.x) / dir.x) > fabs((start.y - next.y) / dir.y))
			{
				start.x += ((next.y - start.y) / dir.y) * dir.x;
				start.y = next.y;
			}
			else
			{
				start.y += ((next.x - start.x) / dir.x) * dir.y;
				start.x = next.x;
			}
			sf::Vector2i index = sf::Vector2i((start.x) / mTileSize, (start.y) / mTileSize);
			//if a wall is hit the bullets hitpos is updated and the drawn bullet trail is scaled down
			if (mTiles[index.y][index.x]->Type() == 2 || mTiles[index.y][index.x]->Type() == 1)
			{
				hit = true;
				*h = start - halfTile;
				sf::Vector2f diff = *h - p->Location();
				p->GetGun()->SetDrawRange(sqrt(diff.x*diff.x + diff.y*diff.y));
			}
		}
		}
	//entities that have been hit by bullets
	vector<Entity*> hit;
	for each (Entity* e in mEntities)
	{
		//this first part detects for entity collisions with walls
		sf::Vector2i index = sf::Vector2i((e->Location().x + mTileSize / 2) / mTileSize, (e->Location().y + mTileSize / 2) / mTileSize);
		sf::Rect<float> r(e->Location().x - (mTileSize + e->Size().x) / 2, e->Location().y - (mTileSize + e->Size().y) / 2, (mTileSize + e->Size().x), (mTileSize + e->Size().y));
		if (index.x > 0 && index.y > 0 && index.x < mTiles[0].size() && index.y < mTiles.size())
		{
			bool onWall = false;
			//uses the same directions as pathfinding to check the 8 tiles around the entity
			//checks collision against sides before diagonally to ensure correct response
			for each (sf::Vector2i t in mDirections)
			{
				int type = mTiles[index.y + t.y][index.x + t.x]->Type();
				if (type == 2 || type == 1)
				{
					if (r.contains(sf::Vector2f((index.x + t.x) * mTileSize, (index.y + t.y) * mTileSize)))
					{
						if (e->Height() >= mTileSize && type == 2)
						{
							onWall = true;
						}
						else
						{
							//calls collision response method belonging to the entity passing the rectangle collided with
							e->Collision(mTiles[index.y + t.y][index.x + t.x]->Rect());
							r = sf::Rect<float>(e->Location().x - (mTileSize + e->Size().x) / 2, e->Location().y - (mTileSize + e->Size().y) / 2, (mTileSize + e->Size().x), (mTileSize + e->Size().y));
						}
					}
				}
			}
			e->SetOnWall(onWall);
		}
		//checks for player bullets hitting enemies using bullethit function above
		//this check comes after bullets hitting walls check to ensure enemies are not hit through walls
		if (dynamic_cast<Enemy*>(e) != 0)
		{
			float minDist = 10000000;
			Entity* tempHit = 0;
			for each (Player* p in mPlayers){
				for each (sf::Vector2f* h in p->GetGun()->GetBullets())
				{
					std::pair<bool, float> hit = BulletHit(p->Location(), e->Rect(), *h);
					if (hit.first)
					{
						if (minDist > hit.second)
						{
							tempHit = e;
							minDist = hit.second;
							p->GetGun()->SetDrawRange(hit.second - e->Size().x / 2);
							*h = sf::Vector2f();
						}
					}
				}
			}
			//hit entities are added to the hit vector
			if (tempHit != 0)
				hit.push_back(tempHit);
			minDist = 10000000;
			tempHit = 0;
			//checks for enemy bullets hitting players using same method as before
			//enemies only shoot directly at the player and only if they have line of sight
			//so no additional wall collision check is needed
			Enemy* en = static_cast<Enemy*>(e);
			for each (Player* p in mPlayers){
				if (p->Alive())
				{
					for each (sf::Vector2f* h in en->GetGun()->GetBullets())
					{
						std::pair<bool, float> hit = BulletHit(en->Location(), p->Rect(), *h);
						if (hit.first)
						{
							if (minDist > hit.second)
							{
								tempHit = p;
								minDist = hit.second;
								en->GetGun()->SetDrawRange(hit.second - e->Size().x / 2);
								*h = sf::Vector2f();
							}
						}
					}
				}
			}
			if (tempHit != 0)
				hit.push_back(tempHit);
			minDist = 10000000;
			tempHit = 0;
			//checks for turret bullets hitting enemies using the same method
			for each (Turret* t in mTurrets){
				for each (sf::Vector2f* h in t->GetGun()->GetBullets())
				{
					std::pair<bool, float> hit = BulletHit(t->Location(), e->Rect(), *h);
					if (hit.first)
					{
						if (minDist > hit.second)
						{
							tempHit = e;
							minDist = hit.second;
							t->GetGun()->SetDrawRange(hit.second - e->Size().x / 2);
							*h = sf::Vector2f();
						}
					}
				}
			}
			if (tempHit != 0)
				hit.push_back(tempHit);
		}
	}
}

void Level::ProcessInput(sf::Event e)
{
	for each (Player* p in mPlayers)
	{
		p->ProcessInput(e, mCam.Offset(), mCam.Scale());
	}
}