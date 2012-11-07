#include <windows.h>
#include <GL/GL.h>
#include <algorithm>
#include "TAGGL/Inc/Object.h"
#include "TAGGL/Inc/ROEditor.h"

using namespace TAGGL;

IFace::IFace() : mType(0) , mDesc1(0) , mDesc2(0) , mIsTextured(false) , mIsWireframe(false)
{
	memset(&mTexture,0,sizeof(mTexture));
}

FFace::FFace() : mColourR(1.0f) , mColourG(1.0f) , mColourB(1.0f) , mTextureHandle(0) , mIsTextured(false) , mIsScaledSprite(false) , mIsLine(false) ,
					mScaledSpriteSize(0.0f)
{
	memset(&mVertList,0,sizeof(mVertList));
	memset(&mTexture,0,sizeof(mTexture));
	
}

Object::Object()
{
}

Object::~Object()
{
}

void Object::ROOutputToLive(void)
{
	mFaces.clear();
	std::list<IFace>::iterator i;
	for (i = mIFaces.begin() ; i != mIFaces.end() ; i++)
	{
		IFace &iface = *i;
		FFace face;
		unsigned int RGBA = gRISCOSToRGB[iface.mDesc1 & 0xff];
		face.mColourR = float((RGBA >> 24) & 0xff) / 255.0f;
		face.mColourG = float((RGBA >> 16) & 0xff) / 255.0f;
		face.mColourB = float((RGBA >> 8) & 0xff) / 255.0f;

		// Convert polygons to triangle fans
		if (iface.mIsWireframe)
		{
			face.mIsLine = true;
			size_t i;
			for (i=0 ; i < iface.mVertList.size() ; i++)
			{
				int last1 = iface.mVertList[i % iface.mVertList.size()];
				int last2 = iface.mVertList[(i+1) % iface.mVertList.size()];

				face.mVertList[0].mX = float(mIVectors[last1].mX) / 256.0f;
				face.mVertList[0].mY = float(mIVectors[last1].mY) / 256.0f;
				face.mVertList[0].mZ = float(mIVectors[last1].mZ) / 256.0f;

				face.mVertList[1].mX = float(mIVectors[last2].mX) / 256.0f;
				face.mVertList[1].mY = float(mIVectors[last2].mY) / 256.0f;
				face.mVertList[1].mZ = float(mIVectors[last2].mZ) / 256.0f;

				mFaces.push_back(face);
			}

		}
		else if (iface.mVertList.size() >= 3)
		{
			if (iface.mIsTextured)
			{
				face.mTextureHandle = iface.mDesc1;
				face.mIsTextured = true;
				face.mColourR = 1.0f;
				face.mColourG = 1.0f;
				face.mColourB = 1.0f;
			}

			int last1 = iface.mVertList[0];
			int last2 = iface.mVertList[1];
			size_t i;
			for (i=2 ; i < iface.mVertList.size() ; i++)
			{
				face.mVertList[0].mX = float(mIVectors[last1].mX) / 256.0f;
				face.mVertList[0].mY = float(mIVectors[last1].mY) / 256.0f;
				face.mVertList[0].mZ = float(mIVectors[last1].mZ) / 256.0f;

				face.mVertList[1].mX = float(mIVectors[last2].mX) / 256.0f;
				face.mVertList[1].mY = float(mIVectors[last2].mY) / 256.0f;
				face.mVertList[1].mZ = float(mIVectors[last2].mZ) / 256.0f;

				face.mVertList[2].mX = float(mIVectors[iface.mVertList[i]].mX) / 256.0f;
				face.mVertList[2].mY = float(mIVectors[iface.mVertList[i]].mY) / 256.0f;
				face.mVertList[2].mZ = float(mIVectors[iface.mVertList[i]].mZ) / 256.0f;

				if (face.mIsTextured && (i < 4))
				{
					face.mTexture[0][0] = float(iface.mTexture.mX[0]) / 16384.0f;
					face.mTexture[0][1] = float(iface.mTexture.mY[0]) / 16384.0f;

					face.mTexture[1][0] = float(iface.mTexture.mX[i-1]) / 16384.0f;
					face.mTexture[1][1] = float(iface.mTexture.mY[i-1]) / 16384.0f;

					face.mTexture[2][0] = float(iface.mTexture.mX[i]) / 16384.0f;
					face.mTexture[2][1] = float(iface.mTexture.mY[i]) / 16384.0f;
				}
				mFaces.push_back(face);

				last2 = iface.mVertList[i];
			}
		}
		else if (iface.mVertList.size() == 2)
		{
			// No textured lines, obviously :)
			int last1 = iface.mVertList[0];
			int last2 = iface.mVertList[1];
			face.mVertList[0].mX = float(mIVectors[last1].mX) / 256.0f;
			face.mVertList[0].mY = float(mIVectors[last1].mY) / 256.0f;
			face.mVertList[0].mZ = float(mIVectors[last1].mZ) / 256.0f;

			face.mVertList[1].mX = float(mIVectors[last2].mX) / 256.0f;
			face.mVertList[1].mY = float(mIVectors[last2].mY) / 256.0f;
			face.mVertList[1].mZ = float(mIVectors[last2].mZ) / 256.0f;

			face.mIsLine = true;

			mFaces.push_back(face);
		}
		else if (iface.mVertList.size() == 1)
		{
			if (iface.mIsTextured)
			{
				face.mTextureHandle = iface.mDesc1;
				face.mIsTextured = true;
				face.mColourR = 1.0f;
				face.mColourG = 1.0f;
				face.mColourB = 1.0f;
				face.mIsScaledSprite = true;
				face.mScaledSpriteSize = float(iface.mDesc2 & 0xffff) / 256.0f;

				face.mVertList[0].mX = float(mIVectors[iface.mVertList[0]].mX) / 256.0f;
				face.mVertList[0].mY = float(mIVectors[iface.mVertList[0]].mY) / 256.0f;
				face.mVertList[0].mZ = float(mIVectors[iface.mVertList[0]].mZ) / 256.0f;

				mFaces.push_back(face);
			}

		}
	}
}

void Object::Render(const float scaleX, const float scaleY, const float scaleZ)
{
	GLuint lastTexture = 0;
	bool textureOn = false;
	glDisable(GL_TEXTURE_2D);

	size_t i;
	for (i=0; i<mFaces.size(); i++)
	{
		FFace &face = mFaces[i];
		if (face.mIsTextured)
		{
			if (!textureOn)
			{
				glColor3f(1.0f,1.0f,1.0f);
				glEnable(GL_TEXTURE_2D);
				textureOn = true;
			}
			if (lastTexture != face.mTextureHandle)
			{
				glBindTexture(GL_TEXTURE_2D, face.mTextureHandle);
				lastTexture = face.mTextureHandle;
			}
		}
		else
		{
			if (textureOn)
			{
				glDisable(GL_TEXTURE_2D);
				textureOn = false;
			}
			glColor3f(face.mColourR, face.mColourG, face.mColourB);
		}

		if (face.mIsScaledSprite)
		{
			// MPi: TODO: Optimise by flagging for a scaled sprite redraw if the face draw notices any such faces.
			// This is because all the matrix fudging is a little bit expensive.
			GLfloat matrix[16];
			glGetFloatv(GL_MODELVIEW_MATRIX,(GLfloat*)matrix);

			glBegin(GL_QUADS);

			float theSize = face.mScaledSpriteSize * 2;

#if 0
			// This renders multiple crossed quads instead of scaled sprites
			// Remember: Anti-clockwise face = drawn
			glTexCoord2f(0.0f,1.0f);
			glVertex3f((face.mVertList[0].mX * scaleX) - theSize, (face.mVertList[0].mY * scaleY) - theSize, -face.mVertList[0].mZ * scaleZ);

			glTexCoord2f(1.0f,1.0f);
			glVertex3f((face.mVertList[0].mX * scaleX) + theSize, (face.mVertList[0].mY * scaleY) - theSize, -face.mVertList[0].mZ * scaleZ);

			glTexCoord2f(1.0f,0.0f);
			glVertex3f((face.mVertList[0].mX * scaleX) + theSize, (face.mVertList[0].mY * scaleY) + theSize, -face.mVertList[0].mZ * scaleZ);

			glTexCoord2f(0.0f,0.0f);
			glVertex3f((face.mVertList[0].mX * scaleX) - theSize, (face.mVertList[0].mY * scaleY) + theSize, -face.mVertList[0].mZ * scaleZ);

			// Instead of matrix manipulation just draw some extra quads
			glTexCoord2f(0.0f,1.0f);
			glVertex3f((face.mVertList[0].mX * scaleX) + theSize, (face.mVertList[0].mY * scaleY) - theSize, -face.mVertList[0].mZ * scaleZ);

			glTexCoord2f(1.0f,1.0f);
			glVertex3f((face.mVertList[0].mX * scaleX) - theSize, (face.mVertList[0].mY * scaleY) - theSize, -face.mVertList[0].mZ * scaleZ);

			glTexCoord2f(1.0f,0.0f);
			glVertex3f((face.mVertList[0].mX * scaleX) - theSize, (face.mVertList[0].mY * scaleY) + theSize, -face.mVertList[0].mZ * scaleZ);

			glTexCoord2f(0.0f,0.0f);
			glVertex3f((face.mVertList[0].mX * scaleX) + theSize, (face.mVertList[0].mY * scaleY) + theSize, -face.mVertList[0].mZ * scaleZ);




			// Z
			glTexCoord2f(0.0f,0.0f);
			glVertex3f((face.mVertList[0].mX * scaleX), (face.mVertList[0].mY * scaleY) + theSize, (-face.mVertList[0].mZ * scaleZ) - theSize);

			glTexCoord2f(0.0f,1.0f);
			glVertex3f((face.mVertList[0].mX * scaleX), (face.mVertList[0].mY * scaleY) - theSize, (-face.mVertList[0].mZ * scaleZ) - theSize);

			glTexCoord2f(1.0f,1.0f);
			glVertex3f((face.mVertList[0].mX * scaleX), (face.mVertList[0].mY * scaleY) - theSize, (-face.mVertList[0].mZ * scaleZ) + theSize);

			glTexCoord2f(1.0f,0.0f);
			glVertex3f((face.mVertList[0].mX * scaleX), (face.mVertList[0].mY * scaleY) + theSize, (-face.mVertList[0].mZ * scaleZ) + theSize);




			glTexCoord2f(0.0f,0.0f);
			glVertex3f((face.mVertList[0].mX * scaleX), (face.mVertList[0].mY * scaleY) + theSize, (-face.mVertList[0].mZ * scaleZ) + theSize);

			glTexCoord2f(0.0f,1.0f);
			glVertex3f((face.mVertList[0].mX * scaleX), (face.mVertList[0].mY * scaleY) - theSize, (-face.mVertList[0].mZ * scaleZ) + theSize);

			glTexCoord2f(1.0f,1.0f);
			glVertex3f((face.mVertList[0].mX * scaleX), (face.mVertList[0].mY * scaleY) - theSize, (-face.mVertList[0].mZ * scaleZ) - theSize);

			glTexCoord2f(1.0f,0.0f);
			glVertex3f((face.mVertList[0].mX * scaleX), (face.mVertList[0].mY * scaleY) + theSize, (-face.mVertList[0].mZ * scaleZ) - theSize);
#else
			FVector3 right(matrix[0],matrix[4],matrix[8]);
			FVector3 up(matrix[1],matrix[5],matrix[9]);

			FVector3 rpu(right.mX + up.mX,right.mY + up.mY,right.mZ + up.mZ);
			FVector3 rmu(right.mX - up.mX,right.mY - up.mY,right.mZ - up.mZ);

			glTexCoord2f(0.0f,1.0f);
			glVertex3f((face.mVertList[0].mX * scaleX) - (rpu.mX * theSize), (face.mVertList[0].mY * scaleY) - (rpu.mY * theSize), (-face.mVertList[0].mZ * scaleZ) - (rpu.mZ * theSize));

			glTexCoord2f(1.0f,1.0f);
			glVertex3f((face.mVertList[0].mX * scaleX) + (rmu.mX * theSize), (face.mVertList[0].mY * scaleY) + (rmu.mY * theSize), (-face.mVertList[0].mZ * scaleZ) + (rmu.mZ * theSize));

			glTexCoord2f(1.0f,0.0f);
			glVertex3f((face.mVertList[0].mX * scaleX) + (rpu.mX * theSize), (face.mVertList[0].mY * scaleY) + (rpu.mY * theSize), (-face.mVertList[0].mZ * scaleZ) + (rpu.mZ * theSize));

			glTexCoord2f(0.0f,0.0f);
			glVertex3f((face.mVertList[0].mX * scaleX) - (rmu.mX * theSize), (face.mVertList[0].mY * scaleY) - (rmu.mY * theSize), (-face.mVertList[0].mZ * scaleZ) - (rmu.mZ * theSize));

#endif
			
			glEnd();
		}
		else if (face.mIsLine)
		{
			glBegin(GL_LINES);

			int j;
			for (j = 0; j < 2 ; j++)
			{
				glVertex3f(face.mVertList[j].mX * scaleX, face.mVertList[j].mY * scaleY, -face.mVertList[j].mZ * scaleZ);
			}

			glEnd();
		}
		else
		{
			glBegin(GL_TRIANGLES);

			int j;
			for (j = 2; j >= 0 ; j--)
			{
				if (face.mIsTextured)
				{
					glTexCoord2f(face.mTexture[j][0],face.mTexture[j][1]);
				}

				glVertex3f(face.mVertList[j].mX * scaleX, face.mVertList[j].mY * scaleY, -face.mVertList[j].mZ * scaleZ);
			}

			glEnd();
		}
	}

}
