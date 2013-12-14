/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */


function init() {
    window.mAmbientLightManager = new AmbientLightManager();
}

if (window.location.href.indexOf('?channel=android') === -1) {

    window.onload = init;
}
else {
    console.log('android');
}