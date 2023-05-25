

#pragma once



#include <obs/input.h>
#include <obs/ObjModel.h>
#include <obs/object.h>



class Player : public Object
{
public:

	Player()
	{}


	Player(ObjModel *model)
	{
		this->model = model;
	}


	void move(float deltaTime)
	{
		checkInputs();

		model->rotate(0, currentTurnSpeed * deltaTime, 0);
		updateOrientation(0, currentTurnSpeed * deltaTime, 0);


		float distance = currentSpeed * deltaTime;
		float dx = distance * sin(glm::radians(orientation.y));
		float dz = distance * cos(glm::radians(orientation.y));

		model->translate(0, 0, distance);
		updatePosition(-dx, 0, -dz);

		// std::cout << position.x << " " << position.y << " " << position.z << "\n";
		std::cout << orientation.x << " " << orientation.y << " " << orientation.z << "\n";
 
		upwardSpeed += GRAVITY * deltaTime;

		model->translate(0, upwardSpeed * deltaTime, 0);
		updatePosition(0, upwardSpeed*deltaTime, 0);

		if(position.y < TERRAIN_HEIGHT)
		{
			upwardSpeed = 0;

			model->translate(0, -position.y, 0);
			updatePosition(0, -position.y, 0);

			isInAir = false;
		}
	}


private:

	float RUN_SPEED  = 20.0f;
	float TURN_SPEED = 160.0f;
	float GRAVITY    = -50;
	float JUMP_POWER = 30;
	float TERRAIN_HEIGHT = 0;

	bool isInAir = false;

	float currentSpeed = 0;
	float currentTurnSpeed = 0;
	float upwardSpeed = 0;

	ObjModel* model;


	void checkInputs()
	{
	    if(Input::GetKey(Input::KEY_W))
	        currentSpeed = RUN_SPEED;
	    else if(Input::GetKey(Input::KEY_S))
	        currentSpeed = -RUN_SPEED;
	    else
	    	currentSpeed = 0;



	    if(Input::GetKey(Input::KEY_A))
	        currentTurnSpeed = TURN_SPEED;
	    else if(Input::GetKey(Input::KEY_D))
	        currentTurnSpeed = -TURN_SPEED;
	    else
	    	currentTurnSpeed = 0;



	    if(Input::GetKey(Input::KEY_SPACE))
	    	jump();
	}

	void jump()
	{
		if(!isInAir)
		{
			upwardSpeed = JUMP_POWER;
			isInAir = true;
		}
	}

};