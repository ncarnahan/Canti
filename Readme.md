# Canti

Canti is a rendering engine developed for my independent study class.

## Supported Features

- Blinn-Phong Shading
- Normal Mapping (including Tangent Generation)
- Draw Call Sorting
- Shadow Mapping (Directional and Spot Lights)
- Deferred Shading

## Screenshots

### Normal Mapping

![Normal Mapping](http://i.imgur.com/K8Gm6ky.png)

### Draw Call Sorting

Transparent objects are sorted back to front for proper blending. Opaque objects are sorted front to back to assist with early Z-testing.

![Draw Call Sorting](http://i.imgur.com/ef5MZoS.png)

### Shadow Mapping

Shadows use a standard shadow mapping technique. Here is an example of directional light shadows. Spot light shadows are also supported.

![Directional Shadows](http://i.imgur.com/ozyHPQF.png)
