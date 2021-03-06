//
//  VoxelMesh.h
//  emptyExample
//
//  Created by James George on 4/10/14.
//
//

#pragma once

#include "ofMain.h"

class VoxelMesh {
  public:
	VoxelMesh();
		
	
	void setup();
	void update();
	void draw();

	void reloadShaders();	
	void regenerateGeometry();
	bool shouldRegenerate;

	int numVoxels;
	float voxelWidth;
		
	float sphereRadius;
	float spherePercent;
	
	float minDistance;
	float maxDistance;
	
	float twistFactorY;
	float twistSpeedY;
	
	float twistFactorX;
	float twistSpeedX;
	
	float spiralFactor;
	
	ofVec4f noiseDistort;
	float noiseDensity;
	float noiseSpeed;

  protected:
	float noisePosition;
	float twistPositionX;
	float twistPositionY;
	
	ofVbo pointVbo;
	ofVbo lineVbo;
	int nPointIndices;
	int nLineIndices;
	
	int currentNumVoxels;
	float currentVoxelWidth;
	
	ofShader pointShader;
	ofShader lineShader;
};