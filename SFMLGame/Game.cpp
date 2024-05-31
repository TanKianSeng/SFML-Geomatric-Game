#include "Game.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>



Game::Game(const std::string& config)
{
	init(config);
}
void Game::init(const std::string& config)
{
	//TODO: read in config file here
	// use the premade PlayerConfig, EnemyConfig, BulletConfig variables
	std::ifstream fin(config);
	
	if (fin.is_open())
	{
		std::string line;
		std::string player = "Player";
		std::string enemy = "Enemy";
		std::string bullet = "Bullet";
		std::string window = "Window";
		std::string font = "Font";
		
		//Read the file line by line
		while(std::getline(fin,line))
		{
			if (line.find(window) != std::string::npos)
			{
				line.erase(0, line.find(window) + window.length());
				std::istringstream data(line);
				data >> m_windowConfig.W >> m_windowConfig.H >> m_windowConfig.FL >> m_windowConfig.FS;
			}
			if (line.find(player) != std::string::npos)
			{
				line.erase(0, line.find(player) + player.length());
				std::istringstream data(line);
				data >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S >> m_playerConfig.FR >> m_playerConfig.FG >> m_playerConfig.FB >> m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB
					 >> m_playerConfig.OT >> m_playerConfig.V;	
			}
			if (line.find(enemy) != std::string::npos)
			{
				line.erase(0, line.find(enemy) + enemy.length());
				std::istringstream data(line);
				data >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX >> m_enemyConfig.OR >> m_enemyConfig.OG >> m_enemyConfig.OB >> m_enemyConfig.OT
					>> m_enemyConfig.VMIN >> m_enemyConfig.VMAX >> m_enemyConfig.L >> m_enemyConfig.SI;
			}
			if (line.find(bullet) != std::string::npos)
			{
				line.erase(0, line.find(bullet) + bullet.length());
				std::istringstream data(line);
				data >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S >> m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB >> m_bulletConfig.OR >> m_bulletConfig.OG 
					 >> m_bulletConfig.OB >> m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L;
			}
			if (line.find(font) != std::string::npos)
			{
				line.erase(0, line.find(font) + font.length());
				std::istringstream data(line);
				data >> m_fontConfig.F >> m_fontConfig.S >> m_fontConfig.FR >> m_fontConfig.FG >> m_fontConfig.FB;
			}
		}
		fin.close();
	}
	else
	{
		std::cerr << "Failed to open the config.txt" << "\n";
	}
		
	//set up default window parameters
	m_window.create(sf::VideoMode(m_windowConfig.W, m_windowConfig.H), "Assignment 2");
	m_window.setFramerateLimit(m_windowConfig.FL);

	spawnPlayer();
	
	
}

void Game::setPaused(bool paused)
{	
	if (paused)
	{		
		m_invisible = 255;
	}
	else
	{
		m_invisible = 0;
	}	
}

void Game::sMovement()
{
	m_player->cTransform->velocity = { 0,0 };
	if (m_player->cInput->up)
	{
		m_player->cTransform->velocity.y = -5;
	}
	if (m_player->cInput->down)
	{
		m_player->cTransform->velocity.y = 5;
	}
	if (m_player->cInput->left)
	{
		m_player->cTransform->velocity.x = -5;
	}
	if (m_player->cInput->right)
	{
		m_player->cTransform->velocity.x = 5;
	}
}

void Game::sUserInput()
{
	//m_player->cTransform->velocity.x
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		//this event triggers when the window is closed
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}

		//this event is triggered when a key is pressed
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				std::cout << "W ket Pressed\n";
				m_player->cInput->up = true;
				//TODO: set player's input component "up" to true
				break;
			case sf::Keyboard::A:
				std::cout << "A ket Pressed\n";
				m_player->cInput->left = true;
				//TODO: set player's input component "up" to true
				break;
			case sf::Keyboard::S:
				std::cout << "S ket Pressed\n";
				m_player->cInput->down = true;				
				//TODO: set player's input component "up" to true
				break;
			case sf::Keyboard::D:
				std::cout << "D ket Pressed\n";
				m_player->cInput->right = true;				
				//TODO: set player's input component "up" to true
				break;
			case sf::Keyboard::P:
				if (m_paused)
				{
					m_paused = false;
				}
				else
				{
					m_paused = true;
				}
				setPaused(m_paused);
				break;
			default:break;
			}
		}
		
		// this event is triggered when a key is released
		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				std::cout << "W key Released\n";
				m_player->cInput->up = false;
				//TODO: set player's input component "up" to false
				break;
			case sf::Keyboard::A:
				std::cout << "A key Released\n";
				m_player->cInput->left = false;
				//TODO: set player's input component "up" to false
				break;
			case sf::Keyboard::S:
				std::cout << "S key Released\n";
				m_player->cInput->down = false;
				//TODO: set player's input component "up" to false
				break;
			case sf::Keyboard::D:
				std::cout << "D key Released\n";
				m_player->cInput->right = false;
				//TODO: set player's input component "up" to false
				break;
			default:break;
			}
		}
		
		if (!m_paused)
		{
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
				}
				if (event.mouseButton.button == sf::Mouse::Right)
				{							
					spawnSpecialWeapon(m_player);		
				}
			}
		}
	}
}

void Game::sLifeSpan()
{
	for (auto b : m_entities.getEntities("bullet"))
	{
		b->cLifespan->remaining -= 5.1f;
		b->cShape->circle.setFillColor(sf::Color(255, 255, 255, b->cLifespan->remaining));
		b->cShape->circle.setOutlineColor(sf::Color(255, 0, 0, b->cLifespan->remaining));

		if (b->cLifespan->remaining <= 0)
		{
			b->destroy();
		}
	}

	for (auto b : m_entities.getEntities("specialBullet"))
	{
		b->cLifespan->remaining -= 5.1f;
		b->cShape->circle.setFillColor(sf::Color(255, 0, 0, b->cLifespan->remaining));
		b->cShape->circle.setOutlineColor(sf::Color(255, 255, 255, b->cLifespan->remaining));

		if (b->cLifespan->remaining <= 0)
		{
			b->destroy();
			if (m_spTrigger < 8)
			{
				spawnSpecialWeapon(b);
				m_spTrigger++;
			}

		}



	}

	for (auto se : m_entities.getEntities("smallEnemy"))
	{
		se->cLifespan->remaining -= 5.1f;
		se->cShape->circle.setFillColor(sf::Color(se->cShape->circle.getFillColor().r, se->cShape->circle.getFillColor().g, se->cShape->circle.getFillColor().b, se->cLifespan->remaining));
		se->cShape->circle.setOutlineColor(sf::Color(se->cShape->circle.getOutlineColor().r, se->cShape->circle.getOutlineColor().g, se->cShape->circle.getOutlineColor().b, se->cLifespan->remaining));
		se->cShape->circle.getFillColor().r;

		if (se->cLifespan->remaining <= 0)
		{
			se->destroy();
		}
	}

	if (!m_entities.checkEntitiesExist("specialBullet"))
	{
		m_spTrigger = 0;
	}
}

void Game::sRender()
{
	//TODO: change the code below to draw ALL the entities
	//	sample drawing of the player Entity that we have created
	m_window.clear();

	for (auto e : m_entities.getEntities())
	{
		//set the position of the shape based on the entity's transform ->pos
		e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);

		//set the position of the shape based on the entity's transform ->pos
		e->cTransform->angle += 1.0f;
		e->cShape->circle.setRotation(e->cTransform->angle);
		if (!m_paused)
		{
			e->cTransform->pos.x += e->cTransform->velocity.x;
			e->cTransform->pos.y += e->cTransform->velocity.y;
		}
		

		//draw the entity's sf::CIrcleShape
		m_window.draw(e->cShape->circle);
	}

	
	
	spawnUi();
	m_window.display();
}

void Game::sEnemySpawner()
{
	if (m_currentFrame == m_EnemySpawnTimeBuffer)
	{
		spawnEnemy();
	}
	
}

void Game::sCollision()
{
	for (auto e : m_entities.getEntities("enemy"))
	{
		if (e->cTransform->pos.x + e->cCollision->radius >= m_window.getSize().x)
		{
			e->cTransform->velocity.x = e->cTransform->velocity.x * -1;			
		}
		if (e->cTransform->pos.x - e->cCollision->radius <= 0)
		{
			e->cTransform->velocity.x = e->cTransform->velocity.x * -1;			
		}
		if (e->cTransform->pos.y + e->cCollision->radius >= m_window.getSize().y)
		{
			e->cTransform->velocity.y = e->cTransform->velocity.y * -1;		
		}
		if (e->cTransform->pos.y - e->cCollision->radius <= 0)
		{
			e->cTransform->velocity.y = e->cTransform->velocity.y * -1;		
		}
	}


	
	for (auto b : m_entities.getEntities("bullet"))//for every bullet in vector collection
	{
		for (auto e : m_entities.getEntities("enemy"))//for every enemy in vector collection
		{
			Vec2 diff = b->cTransform->pos - e->cTransform->pos;   //calculate the distance between bullet and enemy 
			float distSq = diff.distNoSqrt();//apply squareroot function on that distance calculation
			float totalR = (b->cCollision->radius + e->cCollision->radius) * (b->cCollision->radius + e->cCollision->radius); //using both enemy and bullet radius to calculate the collision point
			if (distSq < totalR) //if they are collide
			{
				b->destroy(); //bullet destroy
				spawnSmallEnemies(e); //enemy will spawn the fragment of itselfs
				e->destroy(); // enemy destroy
				m_player->cScore->score += 500; //this very easy to understand lah
				
				
			}
		}
	}	

	for (auto b : m_entities.getEntities("specialBullet"))
	{
		for (auto e : m_entities.getEntities("enemy"))
		{
			Vec2 diff = b->cTransform->pos - e->cTransform->pos;
			float distSq = diff.distNoSqrt();
			float totalR = (b->cCollision->radius + e->cCollision->radius) * (b->cCollision->radius + e->cCollision->radius);
			if (distSq < totalR)
			{
				
				spawnSmallEnemies(e);
				e->destroy();
				m_player->cScore->score += 500;


			}
		}
	}

	for (auto e : m_entities.getEntities("enemy"))
	{
		Vec2 diff = m_player->cTransform->pos - e->cTransform->pos;
		float distSq = diff.distNoSqrt();
		float totalR = (m_player->cCollision->radius + e->cCollision->radius) * (m_player->cCollision->radius + e->cCollision->radius);
		if (distSq < totalR)
		{
			m_player->cTransform->pos.x = m_window.getSize().x/2;
			m_player->cTransform->pos.y = m_window.getSize().y/2;
			spawnSmallEnemies(e);
			e->destroy();

		}
	}
	
	

	
	if (m_player->cTransform->pos.x + m_player->cCollision->radius >= m_window.getSize().x)
	{
		m_player->cInput->right = false;
	}
	if (m_player->cTransform->pos.x - m_player->cCollision->radius <= 0)
	{
		m_player->cInput->left = false;
	}
	if (m_player->cTransform->pos.y + m_player->cCollision->radius >= m_window.getSize().y)
	{
		m_player->cInput->down = false;
	}
	if (m_player->cTransform->pos.y - m_player->cCollision->radius <= 0)
	{
		m_player->cInput->up = false;
	}
}

void Game::spawnUi()
{
	sf::Font font;
	font.loadFromFile(m_fontConfig.F);
	sf::String labelScore = "Your score : "+ std::to_string(m_player->cScore->score);
	sf::Text textScore(labelScore, font, m_fontConfig.S);
	textScore.setFillColor(sf::Color(m_fontConfig.FR, m_fontConfig.FG, m_fontConfig.FB));
	textScore.setFillColor(sf::Color::White);
	textScore.setPosition(0,0);
	
	
	
	sf::String labelPause = "Game Pause";
	sf::Text textPause(labelPause, font, 60);
	textPause.setFillColor(sf::Color(m_fontConfig.FR, m_fontConfig.FG, m_fontConfig.FB,m_invisible));
	textPause.setPosition(m_window.getSize().x / 2 - textPause.getGlobalBounds().width/2, m_window.getSize().y / 2 - textPause.getGlobalBounds().height / 2);
	
	m_window.draw(textScore);
	m_window.draw(textPause);
	
	
	

}

// respawn the player in the middle of the screen
void Game::spawnPlayer()
{
	//TODO: Finish adding all properties of the player woth the correct value from the config 
	//We create every entity by calling EntityManager, so we use 'auto' to save typing
	auto entity = m_entities.addEntity("player");

	//Give this entity a Transform so it spawn at (200,200) with velocity (1,1) and angle 0 
	float wx = m_window.getSize().x / 2.0f;
	float wy = m_window.getSize().y / 2.0f;
	entity->cTransform = std::make_shared<CTransform>(Vec2(wx,wy), Vec2(m_playerConfig.S,m_playerConfig.S), 0.0f);

	//The entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 4
	entity->cShape = std::make_shared<CShape>(m_playerConfig.SR, m_playerConfig.V, sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB), sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);
	
	//Add an input component to the player so that we can use inputs
	entity->cInput = std::make_shared<CInput>();

	entity->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);
	entity->cScore = std::make_shared<CScore>(0);

	//Since we want this entity to be our player, set our Game's player variable to be this Entity
	//This goes slightly against the EntityManager paradigm, but we use player so much it's worth it
	m_player = entity;

}

void Game::spawnEnemy()
{
	//TODO: Finish adding all properties of the player woth the correct value from the config 
	//We create every entity by calling EntityManager, so we use 'auto' to save typing
	auto entity = m_entities.addEntity("enemy");
	
	//Give this entity a Transform so it spawn at (200,200) with velocity (1,1) and angle 0 
	//giving the rand()function seeding, means different random sequence wil be generated each time
	//random y position within the scope of window
	

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> randX(m_enemyConfig.CR, m_window.getSize().x - m_enemyConfig.CR);
	std::uniform_real_distribution<float> randY(m_enemyConfig.CR, m_window.getSize().y - m_enemyConfig.CR);
	std::uniform_real_distribution<float> randVelocity(m_enemyConfig.SMIN, m_enemyConfig.SMAX);
	std::uniform_real_distribution<float> randomVertices(m_enemyConfig.VMIN, m_enemyConfig.VMAX);
	std::uniform_real_distribution<float> randomColor(0,m_enemyConfig.OR);
	std::uniform_real_distribution<float> randomSpawnTime(m_enemyConfig.SI,m_enemyConfig.SI);
	

	
	
	entity->cTransform = std::make_shared<CTransform>(Vec2(randX(gen),randY(gen)), Vec2(randVelocity(gen), randVelocity(gen)), 0.0f);
	entity->cShape = std::make_shared<CShape>(m_enemyConfig.SR, randomVertices(gen), sf::Color(randomColor(gen), randomColor(gen), randomColor(gen)), sf::Color(randomColor(gen), randomColor(gen), randomColor(gen)), m_enemyConfig.OT);
	entity->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);
	m_EnemySpawnTimeBuffer = m_currentFrame + randomSpawnTime(gen);	
	
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> fallenEnemy)
{
	float degree = 360 / fallenEnemy->cShape->circle.getPointCount();
	float initDegree = 0;
	for (size_t i = 0; i < fallenEnemy->cShape->circle.getPointCount(); i++)
	{
		Vec2 velocity = velocity.angleVelocity(initDegree);
		
		auto se = m_entities.addEntity("smallEnemy");
		se->cShape = std::make_shared<CShape>(fallenEnemy->cShape->circle.getRadius()/2, fallenEnemy->cShape->circle.getPointCount(), sf::Color(fallenEnemy->cShape->circle.getFillColor()), sf::Color(fallenEnemy->cShape->circle.getOutlineColor()), fallenEnemy->cShape->circle.getOutlineThickness()/2);
		se->cLifespan = std::make_shared<CLifespan>(255);
		se->cCollision = std::make_shared<CCollision>(fallenEnemy->cShape->circle.getRadius() / 2);
		se->cTransform = std::make_shared<CTransform>(Vec2(fallenEnemy->cTransform->pos), Vec2(velocity), 0.0f);

		initDegree += degree;
		
	}
	
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos)
{
	auto bullet = m_entities.addEntity("bullet");
	Vec2 diff = mousePos - entity->cTransform->pos;
	
	float dist = diff.dist();
	Vec2 n = (diff/dist);
	Vec2 velocity(m_bulletConfig.S * n.x , m_bulletConfig.S * n.y);
	
	bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, velocity, 0);
	bullet->cShape = std::make_shared<CShape>(m_bulletConfig.SR, m_bulletConfig.V, sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB), sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);
	bullet->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L);
	bullet->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	int bulletSplit = 8;
	float degree = 360 / bulletSplit;
	float initDegree = 0;
	for (size_t i = 0; i < bulletSplit; i++)
	{
		Vec2 velocity = velocity.angleVelocity(initDegree);

		
		auto se = m_entities.addEntity("specialBullet");
		se->cShape = std::make_shared<CShape>(m_bulletConfig.SR, bulletSplit, sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB), sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);
		se->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L);
		se->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
		se->cTransform = std::make_shared<CTransform>(Vec2(entity->cTransform->pos), Vec2(velocity.x * m_bulletConfig.S/3,velocity.y * m_bulletConfig.S/3), 0);
		
		initDegree += degree;

	}
}



void Game::run()
{
	//TODO: add pause functionality in here
	//some systems should function while paused (rendering)
	//some systems shouldn't (movement/ input)
	while (m_running)
	{
		m_entities.update();
		sRender();
			
		sEnemySpawner();	
		sMovement();
		sUserInput();	
			
		if (!m_paused)
		{
			sCollision();
			sLifeSpan();
			m_currentFrame++;
		}
		
	}
}
