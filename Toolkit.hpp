#define PVector sf::Vector2f
#define PI 3.1415926

int random(double limit, int base=0)
{
    int r = rand() % int(limit) + base;
    if(r >= limit)
    {
        r = limit;
    }
    return r;
}

std::string random_str(int length)
{
    std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    std::string str = "";
    for(int i = 0; i < length; i++)
    {
        str += alphabet.at(random(26));
    }
    return str;
}

std::string to_string(float number)
{
    std::stringstream ss(std::stringstream::in | std::stringstream::out);
    ss << number;
    return ss.str();
}

PVector getOffset(PVector a, PVector b)
{
    PVector offset(b.x - a.x, b.y - a.y);
    return offset;
}

float getDistance(PVector a, PVector b)
{
    PVector offset = getOffset(a, b);
    float distance = sqrt(pow(offset.x, 2) + pow(offset.y, 2));
    return distance;
}

/*---------------------------------------------------*/

class AdjustableView: public sf::View
{
public:
    void handleEvent(sf::Event& event)
    {
        if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::S){
                setSize(getSize().x * 1.25, getSize().y * 1.25);
            }
            if(event.key.code == sf::Keyboard::W){
                setSize(getSize().x / 1.25, getSize().y / 1.25);
            }
        }
    }
};

/*---------------------------------------------------*/

class Button: public sf::Text
{
    bool active;
public:
    Button()
    {
        active = false;
    }

    void handleEvent(sf::Event& event, PVector mousePos)
    {
        if(event.type == sf::Event::MouseButtonPressed)
        {
            if(getGlobalBounds().contains(mousePos))
            {
                active = true;
            }
        }
    }

    bool selected()
    {
        return active;
    }

    void update()
    {
        active = false;
    }
};

/*---------------------------------------------------*/

class Line: public sf::RectangleShape
{
public:
    void setPoints(PVector a, PVector b)
    {
       float d = getDistance(a, b);

       setSize(PVector(d, 2));
       setPosition(a);

       PVector offset = getOffset(a, b);
       float angle = atan2(offset.y, offset.x);
       setRotation(angle*180/PI);
    }
};

/*---------------------------------------------------*/

PVector normalize(PVector v)
{
    PVector p;
    if(abs(v.x) > 1)
    {
        p.x = v.x / abs(v.x);
    }
    if(abs(v.y) > 1)
    {
        p.y = v.y / abs(v.y);
    }
    return p;
}

PVector mult(PVector v, float n)
{
    PVector p;
    p.x = v.x*n;
    p.y = v.y*n;
    return p;
}

PVector sub(PVector a, PVector b)
{
    PVector d;
    d.x = a.x - b.x;
    d.y = a.y - b.y;
    return d;
}

PVector add(PVector a, PVector b)
{
    PVector s;
    s.x = a.x + b.x;
    s.y = a.y + b.y;
    return s;
}

PVector limit(PVector v, float l)
{
    PVector p;
    if(abs(v.x) > l)
    {
        p.x = l * v.x/abs(v.x);
    }
    if(abs(v.y) > l)
    {
        p.y = l * v.y/abs(v.y);
    }
    return p;
}

/*---------------------------------------------------*/

class Walker: public sf::CircleShape
{
    float timer;
    float period;
    PVector velocity;
    unsigned int step_count;

    void step()
    {
        step_count ++;
        if(random(10) <= 5)
        {
            velocity.x *= -1;
        }
        else
        {
            velocity.y *= -1;
        }
    }

public:
    Walker(float step_time)
    {
        period = step_time;
        timer = 0;

        float speed = 75;
        velocity = PVector(speed, speed);
        step_count = 0;
    }

    void update(float dt)
    {
        timer += dt;
        if(timer >= period)
        {
            timer = 0;
            step();
        }

        move(velocity.x*dt, velocity.y*dt);
    }

    unsigned int get_steps()
    {
        return step_count;
    }
};

/*---------------------------------------------------*/

class CheckerBoard
{
    PVector cellSize;
    PVector boardSize;
    std::vector<sf::Color> colors;
    std::vector<sf::RectangleShape> rects;

    void generateColors(int cLimit)
    {
        for(int i = 0; i < cLimit; i++)
        {
            sf::Color color( random(255, 5), random(255, 5), random(255, 5), random(255, 5) );
            colors.push_back(color);
        }
    }
public:
    CheckerBoard(PVector windowSize, int rows, int col, bool border=false)
    {
        boardSize = windowSize;
        cellSize.x = boardSize.x / rows;
        cellSize.y = boardSize.y / col;

        colors.clear();
        generateColors( random(sqrt(rows + col), 2) );

        int counter = 0;
        for(float i = 0; i < windowSize.x; i+=cellSize.x)
        {
            for(float j = 0; j < windowSize.y; j+=cellSize.y)
            {
                sf::RectangleShape rect;
                rect.setPosition(i, j);
                rect.setSize(cellSize);

                sf::Color color = colors[counter];
                if(border)
                {
                    bool boarderCell = false;
                    if( (i <= 1 || (i/cellSize.x) >= rows-1) || (j <= 1 || (j/cellSize.y) >= col-1) )
                    {
                        boarderCell = true;
                    }

                    if(boarderCell)
                    {
                        color.a = 25;
                    }
                }
                rect.setFillColor(color);
                counter ++;

                if(counter == colors.size())
                {
                    counter = 0;
                }

                rects.push_back(rect);
            }
        }
    }
    void draw(sf::RenderWindow& window)
    {
        for(int i = 0; i < rects.size(); i++)
        {
            window.draw(rects[i]);
        }
    }
    void draw(sf::RenderTexture& texture)
    {
        for(int i = 0; i < rects.size(); i++)
        {
            texture.draw(rects[i]);
        }
    }
};

/*---------------------------------------------------*/

class TileSet
{
    std::vector<sf::Texture> tiles;
public:
    TileSet(sf::Image& src, PVector tileSize)
    {
        sf::Vector2u imgSize = src.getSize();
        for(int y = 0; y < imgSize.y; y+=tileSize.y)
        {
            for(int x = 0; x < imgSize.x; x+=tileSize.x)
            {
                sf::Texture tile;
                tile.loadFromImage(src, sf::IntRect(sf::Vector2i(x, y), sf::Vector2i(tileSize)));
                tiles.push_back(tile);
            }
        }
    }

    unsigned int size()
    {
        return tiles.size();
    }
};
