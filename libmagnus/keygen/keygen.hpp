#pragma once

class Keygen {
public:
	virtual void generate_key() = 0;
	virtual void generate_shared_key() = 0;
};
