// Prénoms, noms et matricule des membres de l'équipe:
// - Prénom1 NOM1 (matricule1)
// - Prénom2 NOM2 (matricule2)


// déclaration d'une structure pour contenir toutes les variables globales
var glob = { };

function handleMouseDown(event)
{
    glob.mouseDown = true;
    glob.positionSourisX = event.clientX-15;
    glob.positionSourisY = event.clientY-15;
}

function handleMouseUp(event)
{
    glob.mouseDown = false;
}

function handleMouseMove(event)
{
    if (!glob.mouseDown) return;
    glob.positionSourisX = event.clientX-15;
    glob.positionSourisY = event.clientY-15;
}

function TPchargerToutesLesTextures()
{
    // Charger une image utilisée comme texture.  (Retourne un objet WebGLTexture)
    glob.texture1 = loadImageTexture( gl, "images/Brouillard.jpg" );
    //glob.texture2 = loadImageTexture( gl, "images/Exploration.jpg" );
    //glob.texture3 = loadImageTexture( gl, "images/Modelisation.jpg" );
}

function TPcreerModele()
{
    // Créer une boîte.  Au retour, 'glob.box' contient une structure avec
    // les VBOs pour les sommets, normales, coordonnées de texture et connectivité.
    glob.box = makeBox( gl );

    // Initialiser les attributs pour les sommets, les normales et les coordonnées de texture
    // (dans le même ordre qu'à l'appel à simpleSetup() dans la fonction TPinitialiser())
    gl.enableVertexAttribArray( 0 );
    gl.bindBuffer( gl.ARRAY_BUFFER, glob.box.normalObject);
    gl.vertexAttribPointer( 0, 3, gl.FLOAT, false, 0, 0 );

    gl.enableVertexAttribArray( 1 );
    gl.bindBuffer( gl.ARRAY_BUFFER, glob.box.texCoordObject);
    gl.vertexAttribPointer( 1, 2, gl.FLOAT, false, 0, 0 );

    gl.enableVertexAttribArray( 2 );
    gl.bindBuffer( gl.ARRAY_BUFFER, glob.box.vertexObject );
    gl.vertexAttribPointer( 2, 3, gl.FLOAT, false, 0, 0 );

    // Lier le tableau de connectivité
    gl.bindBuffer( gl.ELEMENT_ARRAY_BUFFER, glob.box.indexObject );
}

function TPinitialiser()
{
    // Initialiser webgl
    var gl = initWebGL( "tp-canevas" ); // L'identificateur du canevas
    if (!gl) return;

    glob.program = simpleSetup( gl,
                               // Les identificateurs des deux nuanceurs
                               "nuanceurSommets", "nuanceurFragments",
                               // Les attributs utilisés par les nuanceurs (donnés dans le même ordre que leur indice)
                               [ "vNormal", "vTexCoord", "vPosition"],
                               // La couleur de fond et la profondeur
                               [ 0, 0, 0.3, 1 ], 100);

    // Initialiser les variables uniformes pour les nuanceurs
    gl.uniform3f( gl.getUniformLocation( glob.program, "lightDir" ), 0, 0, 1 );
    gl.uniform1i( gl.getUniformLocation( glob.program, "laTexture" ), 0 );

    // Les angles courants de rotation
    glob.angleRotX = 0, glob.angleRotY = 0, glob.angleRotZ = 0;
    // Les incréments à chaque affichage
    glob.incrRotX = 0.2; glob.incrRotY = 0.3; glob.incrRotZ = 0.4;

    // Créer les matrices nécessaires et assigner les assigner dans le programme
    glob.modelViewMatrix = new J3DIMatrix4();
    // glob.u_modelViewMatrixLoc n'est pas utile car modelViewMatrix n'est pas utilisé dans les nuanceurs
    glob.mvpMatrix = new J3DIMatrix4();
    glob.u_modelViewProjMatrixLoc = gl.getUniformLocation( glob.program, "u_modelViewProjMatrix" );
    glob.normalMatrix = new J3DIMatrix4();
    glob.u_normalMatrixLoc = gl.getUniformLocation( glob.program, "u_normalMatrix" );

    // terminer l'initialisation
    TPchargerToutesLesTextures();
    TPcreerModele();

    glob.mouseDown = false;
    glob.positionSourisX = null;
    glob.positionSourisY = null;
    glob.canevas.onmousedown = handleMouseDown;
    glob.canevas.onmouseup = handleMouseUp;
    glob.canevas.onmousemove = handleMouseMove;

    return gl;
}

function TPafficherModele( gl )
{
    // Effacer le canevas
    gl.clear( gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT );

    // Incrémenter les angles de rotation
    glob.angleRotX += glob.incrRotX;  if ( glob.angleRotX >= 360.0 ) glob.angleRotX -= 360.0;
    glob.angleRotY += glob.incrRotY;  if ( glob.angleRotY >= 360.0 ) glob.angleRotY -= 360.0;
    glob.angleRotZ += glob.incrRotZ;  if ( glob.angleRotZ >= 360.0 ) glob.angleRotZ -= 360.0;

    // Construire la matrice de modélisation
    glob.modelViewMatrix.makeIdentity();
    glob.modelViewMatrix.lookat( 0, 0, 7,  0, 0, 0,  0, 1, 0 );
    glob.modelViewMatrix.rotate( glob.angleRotX, 1.0, 0.0, 0.0 );
    glob.modelViewMatrix.rotate( glob.angleRotY, 0.0, 1.0, 0.0 );
    glob.modelViewMatrix.rotate( glob.angleRotZ, 0.0, 0.0, 1.0 );

    // Construire le produit de matrice "modélisation * projection" et la passer aux nuanceurs
    glob.mvpMatrix.load( glob.perspectiveMatrix );
    glob.mvpMatrix.multiply( glob.modelViewMatrix );
    glob.mvpMatrix.setUniform( gl, glob.u_modelViewProjMatrixLoc, false );

    // Construire la matrice de transformation des normales et la passer aux nuanceurs
    glob.normalMatrix.load( glob.modelViewMatrix );
    glob.normalMatrix.invert();
    glob.normalMatrix.transpose();
    glob.normalMatrix.setUniform( gl, glob.u_normalMatrixLoc, false );

    // Activer la texture à utiliser
    gl.bindTexture( gl.TEXTURE_2D, glob.texture1 );

    // Tracer le cube
    gl.drawElements( gl.TRIANGLES, glob.box.numIndices, gl.UNSIGNED_BYTE, 0 );
}

function TPafficherScene(gl)
{
    glob.perspectiveMatrix = new J3DIMatrix4();
    glob.perspectiveMatrix.perspective( 40, glob.canevas.width / glob.canevas.height, 1, 10 );

    // Définir la clôture
    gl.viewport( 0, 0, glob.canevas.width, glob.canevas.height );

    // Tracer le modèle
    TPafficherModele( gl );
}

var requestId;
function TPdebut()
{
    glob.canevas = document.getElementById('tp-canevas');
    //glob.canevas = WebGLDebugUtils.makeLostContextSimulatingCanvas(c);
    // indiquer de perdre le contexte afin de tester
    //glob.canevas.loseContextInNCalls(15);
    glob.canevas.addEventListener('webglcontextlost', handleContextLost, false);
    glob.canevas.addEventListener('webglcontextrestored', handleContextRestored, false);

    var gl = TPinitialiser();
    if ( !gl ) return;

    var displayFunc = function()
    {
        TPafficherScene(gl);
        requestId = window.requestAnimFrame( displayFunc, glob.canevas );
    };
    displayFunc();

    function handleContextLost(e)
    {
        e.preventDefault();
        clearLoadingImages();
        if ( requestId !== undefined )
        {
            window.cancelAnimFrame(requestId);
            requestId = undefined;
        }
    }

    function handleContextRestored()
    {
        TPinitialiser();
        displayFunc();
    }
}
