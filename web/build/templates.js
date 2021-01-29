exports['dynamic_form']=function(obj){
var __t,__p='',__j=Array.prototype.join,print=function(){__p+=__j.call(arguments,'');};
with(obj||{}){
__p+=' <form class="form-horizontal" id="form"> ';
 if(title) { 
__p+=' <h2>'+
((__t=( title ))==null?'':__t)+
'</h2> ';
 } 
__p+=' ';
 for(name in inputs) { 
__p+=' ';
 var input = inputs[name] 
__p+=' <div class="form-group row"> <label class="col-sm-2 col-form-label" for="form_'+
((__t=( name ))==null?'':__t)+
'">'+
((__t=( input.label ))==null?'':__t)+
'</label> <div class="col-sm-10"> ';
 if(input.type=="checkbox") { 
__p+=' <div class="form-check"> <input type="checkbox" class="form-check-input" id="form_'+
((__t=( name ))==null?'':__t)+
'" name="'+
((__t=( name ))==null?'':__t)+
'" value="true" '+
((__t=( (input.value ? "checked " : "") ))==null?'':__t)+
' > </div> ';
 } else if(input.type=="select") { 
__p+=' <select type="'+
((__t=( input.type ))==null?'':__t)+
'" class="form-control" id="form_'+
((__t=( name ))==null?'':__t)+
'" name="'+
((__t=( name ))==null?'':__t)+
'"> ';
 for(i in input.values) { 
__p+=' <option value="'+
((__t=( i ))==null?'':__t)+
'" '+
((__t=( i==input.value ? "selected=true" : "" ))==null?'':__t)+
'> '+
((__t=( input.values[i] ))==null?'':__t)+
' </option> ';
 } 
__p+=' </select> ';
 } else { 
__p+=' <input type="'+
((__t=( input.type ))==null?'':__t)+
'" class="form-control" id="form_'+
((__t=( name ))==null?'':__t)+
'" name="'+
((__t=( name ))==null?'':__t)+
'" value="'+
((__t=( input.value ))==null?'':__t)+
'"> ';
 } 
__p+=' </div> </div> ';
 } 
__p+=' <div class="col-sm-offset-3"> <button type="button" class="btn btn-primary" id="form_submit">Submit</button> </div> </form>';
}
return __p;
};
exports['gpio']=function(obj){
var __t,__p='',__j=Array.prototype.join,print=function(){__p+=__j.call(arguments,'');};
with(obj||{}){
__p+='<form class="form-horizontal" id="form"> <h2>Edit GPIO configuration</h2> <table class="table"> <tr> <th scope="col">Pin number</th> <th scope="col">Pin name</th> <th scope="col">Mode</th> <th scope="col">Pull up (down GPIO 16)</th> </tr> ';
 for(idx in response_data['gpio']) { 
__p+=' <tr> ';
 var pin = response_data['gpio'][idx] 
__p+=' <th> '+
((__t=( idx ))==null?'':__t)+
' </th> <th> <input type="text" name="gpio['+
((__t=( idx ))==null?'':__t)+
'][\'name\']" value="'+
((__t=( pin['name'] ))==null?'':__t)+
'"> </th> ';
 if(isPinSafeToUse(idx)) { 
__p+=' <td> <select name="gpio['+
((__t=( idx ))==null?'':__t)+
'][\'isInput\']"> <option value="true" '+
((__t=( pin['isInput'] ? 'selected' : ''  ))==null?'':__t)+
'> Input </option> <option value="false" '+
((__t=( !pin['isInput'] ? 'selected' : ''  ))==null?'':__t)+
'> Output </option> </select> </td> <td> <input type="checkbox" name="gpio['+
((__t=( idx ))==null?'':__t)+
'][\'pull\']" '+
((__t=( pin['pull'] ? 'checked' : '' ))==null?'':__t)+
' value="1"> </td> ';
 } else { 
__p+=' <td colspan="2">It is not safe to use that pin</td> ';
 } 
__p+=' </tr> ';
 } 
__p+=' </table> <div class="col-sm-offset-3"> <button type="button" class="btn btn-primary" id="form_submit">Submit</button> </div> </form>';
}
return __p;
};
exports['info']=function(obj){
var __t,__p='',__j=Array.prototype.join,print=function(){__p+=__j.call(arguments,'');};
with(obj||{}){
__p+='<table class="table"> ';
 for(i in rows) { 
__p+=' <tr> <th>'+
((__t=( i ))==null?'':__t)+
'</th> <td>'+
((__t=( rows[i] ))==null?'':__t)+
'</td> </tr> ';
 } 
__p+=' </table>';
}
return __p;
};
exports['signin']=function(obj){
var __t,__p='',__j=Array.prototype.join,print=function(){__p+=__j.call(arguments,'');};
with(obj||{}){
__p+='<form class="form-horizontal" id="form"> <h2>Sign in</h2> <div class="form-group row"> <label class="col-sm-2 col-form-label" for="form_login">Login</label> <div class="col-sm-10"> <input type="text" class="form-text-control" id="form_login" name="login"> </div> </div> <div class="form-group row"> <label class="col-sm-2 col-form-label" for="form_password">Password</label> <div class="col-sm-10"> <input type="text" class="form-text-control" id="form_password" name="password"> </div> </div> <div class="row"> <div class="offset-2 col-sm-1"> <button type="submit" class="btn btn-primary">Submit</button> </div> </div> </form>';
}
return __p;
};
exports['users_list']=function(obj){
var __t,__p='',__j=Array.prototype.join,print=function(){__p+=__j.call(arguments,'');};
with(obj||{}){
__p+='<div id="user_list_container"> <table class="table"> <tr> <th>Enabled</th> <th>Login</th> <th>Roles</th> <th>Actions</th> </tr> ';
 for(var i in data['users']) { 
__p+=' ';
 var user = data['users'][i] 
__p+=' ';
 var rolesStr = user['roles'].join(", ") 
__p+=' <tr> <td>'+
((__t=( user['enabled'] ))==null?'':__t)+
'</td> <td>'+
((__t=( user['login'] ))==null?'':__t)+
'</td> <td>'+
((__t=( rolesStr ))==null?'':__t)+
'</td> <td> <a onclick="editUser('+
((__t=( user['enabled'] ))==null?'':__t)+
', \''+
((__t=( user['login'] ))==null?'':__t)+
'\', \''+
((__t=( rolesStr ))==null?'':__t)+
'\')">Edit</a> / <a onclick="removeUser(\''+
((__t=( user['login'] ))==null?'':__t)+
'\')">Remove</a> </td> </tr> ';
 } 
__p+=' </table> <div class="row"> <div class="col-sm-12"> <button type="button" class="btn btn-primary" onclick="addUser()">New user</button> </div> </div> </div> <div id="user_form_container"> <form class="form-horizontal" id="form"> <h2 id="user_form_title"></h2> <div class="form-group row"> <label class="col-sm-2 col-form-label" for="form_enabled">Enabled</label> <div class="col-sm-10"> <div class="form-check"> <input type="checkbox" class="form-check-input" id="form_enabled" name="enabled" value="true"> </div> </div> </div> <div class="form-group row"> <label class="col-sm-2 col-form-label" for="form_login">Login</label> <div class="col-sm-10"> <input type="text" class="form-text-control" id="form_login" name="login"> </div> </div> <div class="form-group row"> <label class="col-sm-2 col-form-label" for="form_roles">Roles</label> <div class="col-sm-10"> <input type="text" class="form-text-control" id="form_roles" name="roles"> <small class="form-text text-muted">Please provide coma separated list.</small> </div> </div> <div class="form-group row"> <label class="col-sm-2 col-form-label" for="form_password">Password</label> <div class="col-sm-10"> <input type="text" class="form-text-control" id="form_password" name="password"> <small class="form-text text-muted">Please provide only if you would like to change user password.</small> </div> </div> <div class="row"> <div class="offset-2 col-sm-1"> <button type="submit" class="btn btn-primary">Submit</button> </div> <div class="col-sm-1"> <button type="reset" class="btn" id="user_form_cancel">Cancel</button> </div> </div> </form> </div>';
}
return __p;
};