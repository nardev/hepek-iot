$(document).ready(function() {

  function renameCloneIdsAndNames(objClone, emptyIT = true) {
    if (!objClone.attr('data-mqtt-command-block-id')) {
      console.error('Cloned object must have \'data-mqtt-command-block-id\' attribute.');
    }

    if (objClone.attr('id')) {
      objClone.attr('id', objClone.attr('id').replace(/\d+$/, function(strId) {
        return parseInt(strId) + 1;
      }));
    }

    objClone.attr('data-mqtt-command-block-id', objClone.attr('data-mqtt-command-block-id').replace(/\d+$/, function(strId) {
      return parseInt(strId) + 1;
    }));

    objClone.find('[id]').each(function() {
      var strNewId = $(this).attr('id').replace(/\d+$/, function(strId) {
        return parseInt(strId) + 1;
      });
      $(this).attr('id', strNewId);

      if ($(this).attr('name')) {
        var strNewName = $(this).attr('name').replace(/\[\d+\]/g, function(strName) {
          strName = strName.replace(/[\[\]']+/g, '');
          var intNumber = parseInt(strName) + 1;
          return '[' + intNumber + ']'
        });
        $(this).attr('name', strNewName);
      }

      if (emptyIT) {
        $(this).val("");
      }

    });

    return objClone;
  }

  $('#add-command').click(function(event) {
    event.preventDefault();
    var clone = cloneAndApppend('div[id^="mqtt-command-block-"]', 'form#mqtt_commands');
    var theId = parseInt(clone.attr('id').replace(/[^\d]/g, ''), 10);
    $('.mqtt-command-form').hide();
    $('.hide-show-form').text("+");
    $('#hide-show-form-' + theId).text("-");
    generateCommandTitle('.mqtt-command-block');
    $('#mqtt-command-form-' + theId).show();
  });

  $(document).on('click', '.delete-command', function(event) {
    event.preventDefault();
    var theId = parseInt($(this).attr('id').replace(/[^\d]/g, ''), 10);
    if ($('div[id^="mqtt-command-block-"]').length > 1) {
      var prevId = theId - 1;
      $('#mqtt-command-form-' + prevId).show();
      $('#mqtt-command-block-' + theId).remove();
    } else {
      console.log("It's only one there...");
    }
  });

  $(document).on('click', '.hide-show-form', function(event) {
    event.preventDefault();
    $('.mqtt-command-form').hide();
    $('.hide-show-form').text("+");
    $(this).text("-");
    var theId = parseInt($(this).attr('id').replace(/[^\d]/g, ''), 10);
    generateCommandTitle('.mqtt-command-block');
    $('div#mqtt-command-form-' + theId).show();
  });

$.getJSON('/eprom/settings.json', function(data) {
    populateForm('mqtt', data.mqtt);
    populateDynamicForm('mqtt_commands', data.mqtt_commands);
  });

  function cloneAndApppend(toClone, appendTo) {
    var clone = $(toClone + ':visible').last().clone();
    renameCloneIdsAndNames(clone);
    $(appendTo).append(clone);
    return clone;
  }

  function populateForm(form_id, form_data) {
    $.each(form_data, function(index, element) {
      $('form#' + form_id).find('input[name="' + index + '"]').val(element);
    });
  }

  function populateDynamicForm(form_id, form_data) {
    var numberCommands = Object.keys(form_data).length;

    $.each(form_data, function(index, element) {
      $.each(element, function(i, e) {
        $('#mqtt-command-form-' + index).find('[name^="' + i + '"]:last').val(e);
      });
      if (index < numberCommands - 1) {
        cloneAndApppend('div[id^="mqtt-command-block-"]', 'form#mqtt_commands');
      }
      $('.mqtt-command-form').hide();
      $('.mqtt-command-form:last').show();
      $('.hide-show-form').text("+");
      $('#hide-show-form-' + index).text("-");
      generateCommandTitle('.mqtt-command-block');
    });
  }

  function generateCommandTitle(block) {
    $(block).each(function() {
      var id = parseInt($(this).attr('id').replace(/[^\d]/g, ''), 10);
      var device_id = $('input[name=device_id]').val();
      $(this).find('[name^=device_id]').val(device_id);
      // var root = $(this).find('[name^=root]').val();
      var type = $(this).find('[name^=type]').val();
      var name = $(this).find('[name^=name]').val();
      var message = $(this).find('[name^=message]').val();
      $("#mqtt-command-title-" + id + " > span").text("/" + device_id + "/" + type + "/" + name + " > [msg: " + message + "]");

    });
  }


  $.fn.hepekObject = function()
  {
    var formId = this.attr('id');
    var trnc = {};
    if(this.attr('data-block')) {
      var block_id = this.attr('data-block');
      $('div[id^="'+block_id+'"]').each(function(i,j){
      	trnc[i] = $(this).find('input,select,textarea,radio').serializeObject();
      });
    } else {
      trnc = $(this).find('input,select,textarea,radio').serializeObject();
    }
    return trnc;
  };

  $.fn.serializeObject = function()
  {
     var o = {};
     var a = this.serializeArray();
     $.each(a, function() {
         var cleanName = this.name.replace('[]', '');
         if (o[cleanName]) {
             if (!o[cleanName].push) {
                 o[cleanName] = [o[cleanName]];
             }
             o[cleanName].push(this.value || '');
         } else {
             o[cleanName] = this.value || '';
         }
     });
     return o;
  };

});

// cannot be called  within document.ready!!!
function updateNeoPixelColor(picker) {
  $('#rgb').text(picker.toRGBString());
  $('input[name="red"]').val(Math.round(picker.rgb[0]));
  $('input[name="green"]').val(Math.round(picker.rgb[1]));
  $('input[name="blue"]').val(Math.round(picker.rgb[2]));
}
