#pragma once


class Input {
private:
	std::map<std::string, std::vector<int> > keys;
	
	std::map<std::string, bool> cache;
	std::map<std::string, bool> cacheOld;
	
	double  x{ 0.0 },  y{ 0.0 };
	double ox{ 0.0 }, oy{ 0.0 };
	double dx{ 0.0 }, dy{ 0.0 };

public:
	Input( void );

	void add( std::string name, std::vector<int> keys );
	bool get( std::string name );
	bool pressed( std::string name );
	bool released( std::string name );

	glm::vec2 mouseDelta( void );
	glm::vec2 mouseAbsolute( void );

	void poll( void );
};
