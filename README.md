# Vulkan-based-renderer
This renderer is Vulkan-based and has implemented features such as model loading, mipmap visualization, and mesh density visualization.

![image](https://github.com/iTzzYiuShaw/Vulkanbased-renderer/assets/110170509/28da53b5-f8b8-4a51-9080-275192cab632)

## Visualizing Mipmaps
 I used the textureQueryLod() function to get the current mipLevel that the sampler is using. 
Next is to get the max mipLevel for a typical texture. By calculating this equation: currentMipLevel/maxMipLevel, we get a percentage that will be helpful to achieve colour visualization. 

![image](https://github.com/iTzzYiuShaw/Vulkanbased-renderer/assets/110170509/732cacae-39e6-437a-895b-30d60fa33866)
![image](https://github.com/iTzzYiuShaw/Vulkanbased-renderer/assets/110170509/6ec580db-19fb-4cf5-b77b-983b5d554c78)


## Mesh density visualization
I define mesh density as the area of triangles in screen space, which is triangle density. For a specific mesh, if the camera moves away from it, the render area of this mesh gets smaller. 
Meanwhile, the number of triangles that form this mesh is still the same, which means that the density of triangles in this mesh is higher, 
and vice versa. To do this, I created a new pipeline that accepts an additional shader, a geometry shader, to calculate the area of each triangle. 

![image](https://github.com/iTzzYiuShaw/Vulkanbased-renderer/assets/110170509/e6cd322d-c811-4224-b0b7-89dc9457546f)
![image](https://github.com/iTzzYiuShaw/Vulkanbased-renderer/assets/110170509/7dbec535-0867-44ce-add7-dbc22965f299)

However, this design is not perfect. As we can see, there is no noticeable color that indicates the mesh with middle density. 
The color is either purple or yellow. Even though there is color transition from blue to yellow, it is not noticeable enough. 
