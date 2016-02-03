#include "stdafx.h"
#include "Level.h"

Level::Level(int tileSize, sf::Vector2i screenSize, Renderer* ren)
	:mTileSize(tileSize), mScreenSize(screenSize), mRen(ren)
{
	mDirections = { sf::Vector2i(0, 0), sf::Vector2i(1, 0), sf::Vector2i(-1, 0), sf::Vector2i(0, 1), sf::Vector2i(0, -1), sf::Vector2i(1, 1), sf::Vector2i(-1, -1), sf::Vector2i(-1, 1), sf::Vector2i(1, -1) };
	//LoadLevel(1);
	Sprite::SetKeep("../Sprites/Arrow.png", true);
	Sprite::SetKeep("../Sprites/Explosion.png", true);
}

//function for deleting vector contents and clearing it
template<typename T>
void Level::ClearVector(std::vector<T>* v)
{
	for each (T o in *v)
	{
		delete(o);
	}
	v->clear();
}

//Load level and wave data from file
void Level::LoadLevel(int level)
{
	//clean up previous level data
	for each (std::vector<Tile*> row in mTiles)
		ClearVector(&row);
	ClearVector(&mTurrets);
	ClearVector(&mPlayers);
	ClearVector(&mCores);
	ClearVector(&mEnemies);
	ClearVector(&mExplosions);
	mEntities.clear();
	mWaves.clear();
	mPlayerLocs.clear();
	mEnemySpawns.clear();
	mTiles.clear();
	mWalls.clear();
	mWave = 0;
	mCurrentLevel = level;

	ifstream myReadFile;
	string line;
	myReadFile.open("../Levels/Level" + std::to_string(level) + ".txt");
	if (myReadFile.is_open())
	{
		mMapSize.y = 0;
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
					mPlayerSpawn = sf::Vector2i(x * mTileSize, mMapSize.y * mTileSize);
					AddEntity(new Core(sf::Vector2f(mPlayerSpawn), mTileSize, mRen));
					type = 0;
				}
				else if (type == 3)
				{
					mEnemySpawns.push_back(sf::Vector2i(x * mTileSize, mMapSize.y * mTileSize));
					type = 0;
				}
				mTiles[mMapSize.y].push_back(new Tile(sf::Vector2i(x * mTileSize, mMapSize.y * mTileSize), mTileSize, type, mRen));
				mTiles.at(mMapSize.y).at(x)->LoadAssets();
			}
			mMapSize.y++;
		}
		AddEntity(new Player(sf::Vector2f(mPlayerSpawn) + sf::Vector2f(0,mTileSize), mTileSize, mRen));
		mPlayerLocs.push_back(mPlayers.back()->Pos());
		//waves stored as a vector for each wave of vectors for each enemy spawn of vectors
		//for each spawn group of pairs of the spawn time and a pair containing enemy type and count
		mWaves.push_back(std::vector<std::vector<std::pair<int, std::pair<int, int>>>>());
		for (int i = 0; i < mEnemySpawns.size(); i++)
			mWaves.back().push_back(std::vector<std::pair<int, std::pair<int, int>>>());
		while (getline(myReadFile, line))
		{
			if (line == "-")
			{
				mWaves.push_back(std::vector<std::vector<std::pair<int, std::pair<int, int>>>>());
				for (int i = 0; i < mEnemySpawns.size(); i++)
					mWaves.back().push_back(std::vector<std::pair<int, std::pair<int, int>>>());
				getline(myReadFile, line);
			}
			if (line.substr(0, 2) != "//")
			{
				int wave[4];
				for (int i = 0, start = -1, end = line.find(',', start + 1); i < 4; start = end, end = line.find(',', start + 1), i++)
					wave[i] = atoi((line.substr(start + 1, end - start - 1)).c_str());
				int a = line.find(',', 2);
				mWaves.back()[wave[1]].push_back(std::pair<int, std::pair<int, int>>(wave[0],std::pair<int, int>(wave[3], wave[2])));
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
	mDefensePhase = true;
	CalcArrowPath(sf::Vector2i());
	mCam = Camera(mPlayers[0]->AimPos(), mPlayers[0]->Pos(), sf::Vector2f(mScreenSize));
	SoundManager::PlayMusic("Building");
	mGameOver = false;
	mWin = false;
}

//makes sure new entities are stored in both the entity vector as well as their own
//also loads assets and other initializations
void Level::AddEntity(Entity* e)
{
	if (dynamic_cast<Player*>(e) != 0)
	{
		mPlayers.push_back(static_cast<Player*>(e));
	}
	else if (dynamic_cast<Enemy*>(e) != 0)
	{
		Enemy* en = static_cast<Enemy*>(e);
		en->SetPath(mArrowPaths[0]);
		mEnemies.push_back(static_cast<Enemy*>(e));
	}
	else if (dynamic_cast<Core*>(e) != 0)
	{
		mCores.push_back(static_cast<Core*>(e));
	}
	else if (dynamic_cast<Turret*>(e) != 0)
	{
		mTurrets.push_back(static_cast<Turret*>(e));
	}
	else if (dynamic_cast<Explosion*>(e) != 0)
	{
		mExplosions.push_back(static_cast<Explosion*>(e));
	}
	e->LoadAssets();
	mEntities.push_back(e);
}

void Level::RemoveEntity(Entity* e)
{
	if (dynamic_cast<Player*>(e) != 0)
	{
		mPlayers.erase(std::find(mPlayers.begin(), mPlayers.end(), static_cast<Player*>(e)));
		delete static_cast<Player*>(e);
	}
	else if (dynamic_cast<Enemy*>(e) != 0)
	{
		mEnemies.erase(std::find(mEnemies.begin(), mEnemies.end(), static_cast<Enemy*>(e)));
		mEnemiesLeft--;
		delete static_cast<Enemy*>(e);
	}
	else if (dynamic_cast<Core*>(e) != 0)
	{
		mCores.erase(std::find(mCores.begin(), mCores.end(), static_cast<Core*>(e)));
		delete static_cast<Core*>(e);
	}
	else if (dynamic_cast<Turret*>(e) != 0)
	{
		mTurrets.erase(std::find(mTurrets.begin(), mTurrets.end(), static_cast<Turret*>(e)));
		delete static_cast<Turret*>(e);
	}
	else if (dynamic_cast<Explosion*>(e) != 0)
	{
		mExplosions.erase(std::find(mExplosions.begin(), mExplosions.end(), static_cast<Explosion*>(e)));
		delete static_cast<Explosion*>(e);
	}
	mEntities.erase(std::find(mEntities.begin(), mEntities.end(), e));
}

void Level::SetTile(Tile* t, int x, int y)
{
	if (mTiles[y][x]->Type() == 2)
		mWalls.erase(std::find(mWalls.begin(), mWalls.end(), mTiles[y][x]));
	delete(mTiles[y][x]);
	mTiles[y][x] = t;
	t->LoadAssets();
	if (t->Type() == 2)
		mWalls.push_back(t);
}

void Level::Update(float t)
{
	t = 0.002f;
	//staggered enemy spawning 
	if (!mDefensePhase)
	{
		//checks if theres an enemy to spawn every 200 milliseconds
		int millisecondsNow = (int)mGameClock.getElapsedTime().asMilliseconds();
		if ((int)mGameClock.getElapsedTime().asMilliseconds() > mNextSpawnTime)
		{
			mNextSpawnTime = millisecondsNow + 200;
			for (int i = 0; i < mEnemySpawns.size(); i++)
			{
				for (int i2 = 0; i2 < mWaves[mWave][i].size(); i2++)
				{
					if (mWaves[mWave][i][i2].first < (int)mGameClock.getElapsedTime().asSeconds())
					{
						if (mWaves[mWave][i][i2].second.first > 0)
						{
							AddEntity(new Enemy(sf::Vector2f(mEnemySpawns[i]) + sf::Vector2f(8 - rand() % 16, 8 - rand() % 16), mTileSize, mWaves[mWave][i][i2].second.second, mRen, mPlayerLocs, i));
							if (mWaves[mWave][i][i2].second.second == 2 && mWeakPath[0].size() > 0)
								mEnemies.back()->SetPath(mWeakPath[0]);
							else
								mEnemies.back()->SetPath(mArrowPaths[i]);
							mWaves[mWave][i][i2].second.first--;
						}
					}
				}
			}
		}
		if (mEnemiesLeft == 0)
		{
			mDefensePhase = true;
			SoundManager::PlayMusic("Building");
			mWave++;
			if (mWave >= mWaves.size())
			{
				mWin = true;
				SoundManager::PlayMusic("Win");
			}
		}
	}

	for each (Player* p in mPlayers)	
	{
		//checks if player wants to place a wall/turret and then places it
		if (mDefensePhase)
		{
			std::pair<int, sf::Vector2i> toPlace = p->GetPlace();
			sf::Vector2i placeIndex = toPlace.second / mTileSize;
			if (toPlace.first == 1)
			{
				if (p->GetCredits() >= 5)
				{
					Tile* t = mTiles[placeIndex.y][placeIndex.x];
					if (t->Type() == 0)
					{
						p->AddCredits(-5);
						if (CalcArrowPath(toPlace.second / mTileSize))
						{
							SetTile(new Tile(toPlace.second, mTileSize, 2, mRen), placeIndex.x, placeIndex.y);
							for (int i = 0; i < 4; i++)
							{
								if (mTiles[placeIndex.y + mDirections[i + 1].y][placeIndex.x + mDirections[i + 1].x]->Type() == 2)
								{
									mTiles[placeIndex.y + mDirections[i + 1].y][placeIndex.x + mDirections[i + 1].x]->SetEdge(i == 0 ? 1 : i == 1 ? 0 : i == 3 ? 2 : 3, true);
									mTiles[placeIndex.y][placeIndex.x]->SetEdge(i, true);
								}
							}
						}
					}
				}
			}
			else if (toPlace.first == 5)
			{
				Tile* t = mTiles[placeIndex.y][placeIndex.x];
				if (t->Type() == 2)
				{
					p->AddCredits(5);
					SetTile(new Tile(toPlace.second, mTileSize, 0, mRen), placeIndex.x, placeIndex.y);
					for (int i = 0; i < mTurrets.size(); i++)
					{
						if (mTurrets[i]->Location() == sf::Vector2f(toPlace.second)){
							p->AddCredits(mTurrets[i]->GetCost());
							RemoveEntity(mTurrets[i]);
							break;
						}
					}
				}
			}
			else if (toPlace.first >= 2)
			{
				if (p->GetCredits() >= 15)
				{
					p->AddCredits(-15);
					Tile* t = mTiles[placeIndex.y][placeIndex.x];
					if (t->Type() == 2 && !t->Occupied())
					{
						AddEntity(new Turret(sf::Vector2f(toPlace.second), mTileSize, toPlace.first - 2, mRen, &mEnemies));
						t->SetOccupied(true);
					}
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
			if(dist < e->GetGun()->GetRange())
			{
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
		}
		e->Shoot(player, minDist);
	}
	//update all entities and camera
	for each (Entity* e in mEntities)
		e->Update(t, mCam.Offset(), mCam.Scale());
	mCam.Update(mMousePos);
}

std::vector<std::pair<sf::Vector2i, float>> Level::CalcPath(sf::Vector2i start, sf::Vector2i goal, std::vector<std::vector<Tile*>>* tiles)
{
	//modified Dijkstras algorithm
	Node* goalNode = mNodes[goal.y*mMapSize.x + goal.x];
	std::vector<std::pair<sf::Vector2i, float>> path;
	priority_queue<Node*, vector<Node*>, Compare> pq;
	for (int i = 0; i < mMapSize.x*mMapSize.y; i++) {
		mNodes[i]->Distance = INT_MAX;
		mNodes[i]->Previous = 0;
		mNodes[i]->Marked = false;
		mNodes[i]->Visited = false;
	}
	Node* startNode = mNodes[start.y*mMapSize.x + start.x];
	if ((*tiles)[start.y][start.x]->Type() != 0)
		return path;
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
				&& (*tiles)[top->Position.y + dir.y][top->Position.x + dir.x]->Type() == 0)
			{
				//makes sure path doesnt go in between diagonal walls and doesnt cut corners
				//makes use of the order it checks the directions
				if (n <= 3 || ((*tiles)[top->Position.y][top->Position.x + dir.x]->Type() == 0 && (*tiles)[top->Position.y + dir.y][top->Position.x]->Type() == 0))
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
		return path;
	//creates paths from the results
	std::vector<std::pair<sf::Vector2i, float>>::iterator it = path.begin();
	path.insert(it, std::pair<sf::Vector2i, float>(goalNode->Position, 0));
	for (Node* temp = goalNode->Previous; temp != 0; temp = temp->Previous)
	{
		it = path.begin();
		std::pair < sf::Vector2i, float > anglePos(temp->Position, atan2(it->first.y - temp->Position.y, it->first.x - temp->Position.x));
		path.insert(it, anglePos);
	}
	return path;
}

//calculates the paths from each enemy spawn to the core
//also takes the position of a possible newly placed tile
//and returns whether or not a path was found
bool Level::CalcArrowPath(sf::Vector2i newPos)
{
	std::vector<std::vector<Sprite*>> tempArrows;
	std::vector<std::vector<std::pair<sf::Vector2i, float>>> tempArrowPaths;
	int spawn = -1;
	for each (sf::Vector2i start in mEnemySpawns)
	{
		spawn++;
		start /= 32;
		sf::Vector2i goal = sf::Vector2i(mCores[min(spawn, (int)mCores.size()-1)]->Location() / 32.f);
		std::vector<std::vector<Tile*>> mTempTiles = mTiles;
		mTempTiles[newPos.y][newPos.x] = new Tile(newPos * mTileSize, mTileSize, 2, mRen);
		

		std::vector<std::pair<sf::Vector2i, float>> path = CalcPath(start, goal, &mTempTiles);
		delete(mTempTiles[newPos.y][newPos.x]);
		if (path.size() == 0)
			return false;
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
	for (int i = 0; i < mArrows.size();i++)
	{
		for (int i2 = 0; i2 < mArrows[i].size(); i2++)
		{
			mArrows[i][i2]->setPosition((sf::Vector2f(mArrowPaths[i][i2].first * 32) - mCam.Offset()) * mCam.Scale());
			mArrows[i][i2]->setScale(mCam.Scale() * .5f, mCam.Scale() * .5f);
		}
	}
	for (int x = 0; x < mTiles.size(); x++)
		for (int y = 0; y < mTiles[0].size(); y++)
			mTiles[x][y]->Draw(mCam.Offset(), mCam.Scale());
	for each (Entity* e in mEntities)
		e->Draw(mCam.Offset(), mCam.Scale());
	mRen->Draw(win);
}

//function to test if a bullet shot from a position and hitting another position will pass through a rectangle target
std::pair<bool, float> BulletHit(sf::Vector2f startPos, sf::IntRect target, sf::Vector2f hitPos)
{
	sf::Vector2f diff = startPos - sf::Vector2f(target.left, target.top);
	sf::Vector2f diff2 = startPos - hitPos;
	sf::Vector2f dir = hitPos - startPos;
	dir /= sqrt(dir.x * dir.x + dir.y * dir.y);
	float dist = min(sqrt(diff.x * diff.x + diff.y * diff.y), sqrt(diff2.x * diff2.x + diff2.y * diff2.y));
	target.top -= target.width / 2;
	target.left -= target.height / 2;
	return std::pair<bool, float>(target.contains(sf::Vector2i(startPos + dir * dist)), dist);
}

std::vector<std::pair<sf::Vector2i, float>> Level::CalcWeakPoint(int s)
{
	int dist = 20000;
	std::vector<std::pair<sf::Vector2i, float>> result;
	for each (Tile* w in mWalls)
	{
		if (!w->Occupied())
		{
			int d = 0;
			sf::Vector2i wallIndex = w->Location() / mTileSize;
			std::vector<std::vector<Tile*>> mTempTiles = mTiles;
			mTempTiles[wallIndex.y][wallIndex.x] = new Tile(w->Location(), mTileSize, 0, mRen);
			std::vector<std::pair<sf::Vector2i, float>> p = CalcPath(wallIndex, sf::Vector2i(mCores[s]->Location()) / mTileSize, &mTempTiles);
			if (p.size() != 0)
			{
				d += p.size();
				p = CalcPath(mEnemySpawns[s] / mTileSize, wallIndex, &mTempTiles);
				if (p.size() != 0)
				{
					d += p.size();
					if (d < dist)
					{
						dist = d;
						result = p;
					}
				}
			}
			delete(mTempTiles[wallIndex.y][wallIndex.x]);
		}
	}
	return result;
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
	vector<std::pair<Entity*, float>> hit;
	for each (Entity* e in mEntities)
	{
		for each (Entity* e2 in mEntities)
		{
			if (e != e2 && e->CanCollide() && e2->CanCollide())
			{
				sf::Vector2f ePos = e->Location();
				e->Collision(e2->Location(), e2->Size().x / 3.f);
				e2->Collision(ePos, e->Size().x / 3.f);
			}
		}

		//this part detects for entity collisions with walls
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
			for each (Player* p in mPlayers)
			{
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
				//hit entities are added to the hit vector
				if (tempHit != 0)
					hit.push_back(std::pair<Entity*, float>(tempHit, p->GetGun()->GetDamage()));
				minDist = 10000000;
				tempHit = 0;
			}
			//checks for enemy bullets hitting players using same method as before
			//enemies only shoot directly at the player and only if they have line of sight
			//so no additional wall collision check is needed
			Enemy* en = static_cast<Enemy*>(e);
			for each (Player* p in mPlayers){
				if (p->Alive())
				{
					for each (sf::Vector2f* h in en->GetGun()->GetBullets())
					{
						std::pair<bool, float> bullethit = BulletHit(en->Location(), p->Rect(), *h);
						if (bullethit.first)
						{
							tempHit = p;
							hit.push_back(std::pair<Entity*, float>(tempHit, en->GetGun()->GetDamage()));
							en->GetGun()->SetDrawRange(bullethit.second - e->Size().x / 2);
							*h = en->Location();
						}
					}
				}
			}
			minDist = 10000000;
			tempHit = 0;
			//checks for turret bullets hitting enemies using the same method
			for each (Turret* t in mTurrets){
				for each (sf::Vector2f* h in t->GetGun()->GetBullets())
				{
					std::pair<bool, float> bullethit = BulletHit(t->Location(), e->Rect(), *h);
					if (bullethit.first)
					{
						if (minDist > bullethit.second)
						{
							tempHit = e;
							if (t->GetType() == 0)
							{
								minDist = bullethit.second;
								t->GetGun()->SetDrawRange(bullethit.second - e->Size().x / 2);
								*h = t->Location();
							}
							else if (t->GetType() == 2)
							{
								hit.push_back(std::pair<Entity*, float>(tempHit, t->GetGun()->GetDamage()));
							}
						}
					}
				}
				if (tempHit != 0)
					hit.push_back(std::pair<Entity*, float>(tempHit, t->GetGun()->GetDamage()));
			}
		}
	}
	std::vector<Entity*> dead;
	//deals damage to each hit enemy
	for each (std::pair<Entity*, float> e in hit)
	{
		if (e.first != 0)
			if (!e.first->Hit(e.second) && std::find(dead.begin(), dead.end(), e.first) == dead.end())
				dead.push_back(e.first);
	}
	for each (Entity* e in dead)
	{
		AddEntity(new Explosion(e->Location(), mTileSize, mRen, 0.4f));
		RemoveEntity(e);
		mPlayers[0]->AddCredits(10);
	}
	//checks if enemies have collided with the core
	for (int i = 0; i < mEnemies.size(); i++)
	{
		if (mEnemies[i]->GoalReached())
		{
			if (mCores.size() > 0)
			{
				//damages core and kills enemy
				if (!mCores[mEnemies[i]->GetCore()]->Hit(2)){
					//RemoveEntity(mCores[mEnemies[i]->GetCore()]); 
					mGameOver = true;
				}
				if (!mEnemies[i]->Hit(10))
				{
					if (mEnemies[i]->Type() == 2)
					{
						AddEntity(new Explosion(mEnemies[i]->Location(), mTileSize, mRen, 1.f));
						SetTile(new Tile(mEnemies[i]->GetGoal() * mTileSize, mTileSize, 0, mRen), mEnemies[i]->GetGoal().x, mEnemies[i]->GetGoal().y);
						CalcArrowPath(sf::Vector2i());
					}
					else
						AddEntity(new Explosion(mEnemies[i]->Location(), mTileSize, mRen, 0.4f));
					RemoveEntity(mEnemies[i]);
				}
				i--;
			}
		}
	}
	for (int i = 0; i < mExplosions.size(); i++)
	{
		if (!mExplosions[i]->Alive())
		{
			RemoveEntity(mExplosions[i]);
			i--;
		}
	}
}

void Level::RestartCurrentWave()
{
	mPlayers[0]->Respawn();
	mDefensePhase = true;
	CalcArrowPath(sf::Vector2i());
	SoundManager::PlayMusic("Building");
	int c = mEnemies.size();
	for (int i = 0; i < c; i++)
	{
		RemoveEntity(mEnemies[0]);
	}
	for each (Core* c in mCores)
	{
		c->ResetHealth();
	}
	mEnemiesLeft = 0;
	mGameOver = false;
}

void Level::ProcessInput(sf::Event e)
{
	if (e.type == sf::Event::KeyReleased && e.key.code == sf::Keyboard::Return && mDefensePhase)
	{
		mDefensePhase = false;
		mGameClock.restart();
		mEnemiesLeft = 0;
		for (int i = 0; i < mWaves[mWave].size(); i++)
		{
			std::vector<std::pair<int, std::pair<int, int>>>::iterator it = mWaves[mWave][i].begin();
			for (int i2 = 0; i2 < mWaves[mWave][i].size(); i2++, it++)
				mEnemiesLeft += it->second.first;
		}
		SoundManager::PlayMusic("Fighting");
		mWeakPath.clear();
		mWeakPath.push_back(CalcWeakPoint(0));
		mNextSpawnTime = 0;
	}
	for each (Player* p in mPlayers)
	{
		p->ProcessInput(e, mCam.Offset(), mCam.Scale());
	}
	if (e.type == sf::Event::MouseMoved)
		mMousePos = sf::Vector2i(e.mouseMove.x, e.mouseMove.y);
}


Player* Level::GetPlayer()
{
	return mPlayers[0];
}