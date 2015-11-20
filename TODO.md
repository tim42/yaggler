
Here are listed some key points for the future of YägGLer:

* Finish the first pass of documentation
  * Add documentation for `bleunw/`
* Add some samples
  * Add samples for klmb
  * Add samples for bleunw
  * Add samples for plain yaggler
* Improves how textures / bindless textures are handled
  * Refactor a bit that code (the one marked with TODOs)
* The process of binding / unbinding buffer begins quietly to be unsustainable/too complex (particularly when speaking of transform feedback, shader storage, ...)
  * Maybe externalizing buffer binding / unbinding could be a better solution...
