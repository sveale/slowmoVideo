/*
slowmoVideo creates slow-motion videos from normal-speed videos.
Copyright (C) 2011  Simon A. Eugster (Granjow)  <simon.eu@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
*/

#ifndef SOURCEFIELD_SV_H
#define SOURCEFIELD_SV_H

class FlowField_sV;

/**
  \brief Counterpart to an Optical Flow field, says where a pixel came from.
  */
class SourceField_sV
{
public:
    /**
      \fn SourceField_sV(int width, int height)
      \brief Creates an empty source field.
      */
    /**
      \fn SourceField_sV(const FlowField_sV *flow, float pos)
      \brief Converts the optical flow to «where did the pixel come from?»
      from the target image's perspective.

      The target image is at position \c pos.

      inpaint() is \em not called by this method.
      */
    /**
      \fn inpaint()
      \brief Fills holes in the source field.

      When generating a source field from an optical flow field, for example:
      \code
0  0  1  1  0  0
       |
       v
0  1  1  1  1  0 \endcode
      then the source field might look like this:
      \code
0  1       -1  0 \endcode
      since the two edges moved one pixel left or right, respectively, and no pixel
      «went to» the part in the middle. This function interpolates those from nearby members
      whose source location is known.
      */
    SourceField_sV(int width, int height);
    SourceField_sV(const FlowField_sV *flow, float pos);
    ~SourceField_sV();

    struct Source {
        float fromX;
        float fromY;
        bool isSet;
        Source() : isSet(false) {}
        Source(float fromX, float fromY) : fromX(fromX), fromY(fromY), isSet(true) {}
        void set(float x, float y) {
            fromX = x; fromY = y; isSet = true;
        }
    };

    Source *m_field;


    inline Source& at(int x, int y)
    {
        return m_field[m_width*y + x];
    }

    void inpaint();

private:
    int m_width;
    int m_height;

    struct SourceSum {
        float x;
        float y;
        int count;
        SourceSum()
        {
            reset();
        }
        Source norm() {
            if (count == 0) { return Source(); }
            else { return Source(x/count, y/count); }
        }

        void operator +=(const Source &other)
        {
            if (other.isSet) {
                count++;
                x += other.fromX;
                y += other.fromY;
            }
        }
        void reset()
        {
            x = 0;
            y = 0;
            count = 0;
        }
    };

};

#endif // SOURCEFIELD_SV_H
