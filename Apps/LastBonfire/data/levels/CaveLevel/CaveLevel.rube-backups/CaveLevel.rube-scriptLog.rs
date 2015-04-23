//Started script log at 04/21/15 13:18:19

addBody(8, '{"awake":true,"type":"dynamic"}');
getBody(8).addFixture(8, '{"density":1,"shapes":[{"radius":0,"type":"polygon"}],"friction":0.2,"vertices":{"x":[-0.5,0.5,0.5,-0.5],"y":[-0.5,-0.5,0.5,0.5]}}');
getBody(8).setPosition(0,0);
getVertex(8,1).select();
getVertex(8,2).select();
getVertex(8,1).setPos(5.37228, -0.5);
getVertex(8,2).setPos(5.37228, 0.5);
getVertex(8,1).deselect();
getVertex(8,2).deselect();
getVertex(8,0).select();
getVertex(8,3).select();
getVertex(8,0).setPos(-18.9952, -0.5);
getVertex(8,3).setPos(-18.9952, 0.5);
getVertex(8,0).deselect();
getVertex(8,3).deselect();
getFixture(8).select();
getShape(8,0).setType(2);
getShape(8,0).setType(3);
getBody(8).select();
getBody(8).deselect();
getFixture(8).deselect();
getFixture(8).delete();
getBody(8).delete();
