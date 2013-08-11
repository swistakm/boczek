.. module:: bacon

Graphics
--------

Bacon provides functions for loading and drawing images and fonts.  It provides a simple 2D API for this, but under the hood is using current generation GPU APIs, which allows us to use sophisticated effects.

While images and fonts can be loaded and queried at any time, the drawing functions can only be called within the scope of your game's :func:`Game.on_tick` method.

At the beginning of each frame (before doing any other rendering in ``on_tick``), you should clear the screen:

.. autofunction:: clear


Images
======

Images can be loaded from most standard file types, for example PNG, BMP, TIFF, JPEG, etc.  For example:

.. literalinclude:: ../examples/image.py
    :emphasize-lines: 3,4,9

.. autoclass:: Image
    :members:

Draw an image with the functions:

.. autofunction:: draw_image

.. autofunction:: draw_image_region

Fonts
=====

To render text, first load a TrueType or OpenType font at a particular point size, then render it with ``draw_string``:

.. literalinclude:: ../examples/font.py
    :emphasize-lines: 3,4,9

.. autoclass:: Font
    :members:

Transform stack
===============

All drawing commands are transformed by the top matrix in a *transform stack* before being submitted for rendering.  By transforming
the top matrix of the transform stack, you affect the position of all subsequent drawing commands.  This can be convenient for rendering
groups of objects with a parent transform, and is necessary to perform rotations, scales and skews on drawing.

This example shows how the :func:`rotate` function can be used to rotate an image, using :func:`translate` to set move
the rotation pivot to the center of the image:

.. literalinclude:: ../examples/transform_rotate.py
    :emphasize-lines: 9,10

.. autofunction:: translate
.. autofunction:: scale
.. autofunction:: rotate

The above functions operate only on the top matrix of the transform stack.  You can save and restore the current transform with 
:func:`push_transform` and :func:`pop_transform`.  The following example shows how the rotation transform does not
affect the text rendered, only the image.

.. literalinclude:: ../examples/transform_push.py
    :emphasize-lines: 11,15

.. autofunction:: push_transform
.. autofunction:: pop_transform
.. autofunction:: set_transform

Color stack
===========

All drawing commands are tinted by the top color in a *color stack* when rendered.  By default the color is white (``(1, 1, 1, 1)``),
so no tinting is applied.  This example shows the image tinted red by modifying the color:

.. literalinclude:: ../examples/color.py
    :emphasize-lines: 8

.. autofunction:: set_color
.. autofunction:: multiply_color

By changing the alpha (the fourth color component), you can make images and text translucent.

Just like the transform stack, the color stack allows you to save and restore the current color.

.. autofunction:: push_color
.. autofunction:: pop_color


Rendering to an image
=====================

By default all rendering is done on framebuffer that is flipped to the main window when your ``on_tick`` method returns.  It is
easy to render to an image, though.  This is useful for compositing elements in a single layer for blending, and for certain
special effects.  

.. literalinclude:: ../examples/render_to_image.py
    :emphasize-lines: 21, 29

.. autofunction:: set_frame_buffer

Shaders
=======

.. note:: TODO: shader example.

.. autofunction:: set_shader

.. autoclass:: Shader
    :members:

Blend modes
===========

The current blend state dictates how new elements are composited into the frame buffer.  The default blend mode is suitable
for compositing images and glyphs with premultiplied alpha; changing the blend mode can be use for special effects.  In the
following example, the same text is rendered over itself in different colors with an additive blend; where the colors 
intersect, the color adds to white:

.. literalinclude:: ../examples/blending.py
    :emphasize-lines: 8

.. autofunction:: set_blending

.. autoclass:: BlendFlags
    :members:
    :undoc-members: