#
# Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
# prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
# Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
#

#! /usr/bin/env python

###
### Show the structure layout of a list of classes.
### Use the showOffsets demo to produce the input files
### for this program.
###
import gtk
import sys
sys.stdout = sys.stderr

_ = lambda x : x

class ShowClass:
    UI_DEFINITION = """
    <ui>
      <menubar name="MenuBar">
        <menu action="file_menu">
          <menuitem action="open"/>
          <menuitem action="quit"/>
        </menu>
        <menu action="view_menu">
          <menuitem action="view4"/>
          <menuitem action="view8"/>
          <menuitem action="view16"/>
        </menu>
      </menubar>
    </ui>
    """
    UI_ACTIONS = (
        # action_name, stock_icon, label, accelerator, tooltip,
        ('file_menu', None, _('_File')),
            ('open', gtk.STOCK_OPEN, None, None, "open"),
            ('quit', gtk.STOCK_QUIT, None, None, "quit"),
        ('view_menu', None, _('_View')),
            ('view4',  None, _('4 byte line'),  None, None, 4, 'area_num_columns'),
            ('view8',  None, _('8 byte line'),  None, None, 8, 'area_num_columns'),
            ('view16', None, _('16 byte line'), None, None, 16, 'area_num_columns'),
    )

    class LayoutInfo:
        def __init__(self, fname):
            self.fname = fname.split("/")[-1].split(".")[1]
            self.membersFromName = {}
            self.sizeFromName = {}
            self.widget = gtk.VBox()
            self.label= gtk.Label()
            self.area = gtk.Table()
            scroll = gtk.ScrolledWindow()
            vtmp = gtk.VBox()
            vtmp.pack_start(self.area, expand=0)
            scroll.add_with_viewport(vtmp)
            self.area.set_homogeneous(True)
            self.widget.pack_start(self.label, expand=0, padding=10)
            self.widget.pack_start(scroll)
            for line in open(fname):
                if line[0] == "*":
                    cname,csize = line[1:].strip().split(",")
                    self.membersFromName[cname] = []
                    self.sizeFromName[cname] = int(csize)
                elif line[0] == "M":
                    off, size, name, typename = line[1:].strip().split(",")
                    off, size = int(off), int(size)
                    self.membersFromName[cname].append( (off, size, name, typename) )
            #scroll = gtk.ScrolledWindow()
            #scroll.add_with_viewport(vbox)
            #vbox.pack_start(area,expand=0, fill=0)
            #vbox2 = gtk.VBox()
            #vbox2.pack_start(scroll)
            #return vbox
        def switch(self, classname, ncols):
            area = self.area
            for c in area.get_children():
                area.remove(c)
            classsize = self.sizeFromName[classname]
            self.label.set_markup("%s : <b>%s</b>"% (self.fname,classname) )
            print "***", classname, classsize #XXX
            area.resize( 1+classsize/ncols, ncols)
            members = self.membersFromName[classname]
            tooltips = gtk.Tooltips()
            for off, origsize, name, typename in members + [(classsize,1,"{END}","")]:
                print "+", off, origsize, name, typename #XXX
                left, top = off%ncols, off//ncols
                size = origsize
                while size > 0:
                    width = min(ncols-left, size)
                    if left == 0:
                        height = max(size//ncols, 1)
                    else:
                        height = 1
                    #print size, left, left+width, top, top+height
                    size -= width*height
                    b = gtk.Button("%s %s" % (name,["","..."][size>0] ) )
                    b.set_use_underline(False)
                    b.connect("clicked", lambda a,n=typename : self.jump_to(n))
                    b.modify_bg(gtk.STATE_NORMAL, gtk.gdk.color_parse("White"))
                    tooltips.set_tip(b, "%s %s %i@%i" % (typename, name, origsize, off))
                    area.attach(b, left, left+width, top, top+height)
                    top += height
                    left = (left+width)%ncols
            tooltips.enable()
            area.show_all()
    
    def __init__(self):
        window = gtk.Window()
        mbox = gtk.VBox()
        hpaned = gtk.HPaned()
        mbox.pack_start(hpaned)
        model = gtk.ListStore(type(""))
        tree = gtk.TreeView(model)
        tree.set_size_request(400,600)
        tree.insert_column_with_attributes(0, "class", gtk.CellRendererText(), text=0)
        scroll = gtk.ScrolledWindow()
        scroll.add(tree)
        hpaned.add(scroll)
        hbox = gtk.HBox()
        hpaned.add(hbox)

        window.set_size_request(800,600)
        window.add(mbox)

        self.window = window
        self.hbox = hbox
        #self.label_cur_class = label_cur_class
        self.model = model
        self.tree = tree
        #self.area = area
        self.layoutInfo = []
        self.history = []
        self.history_index = 0
        self.area_num_columns = 4

        uimanager = gtk.UIManager()
        window.add_accel_group( uimanager.get_accel_group() )
        actiongroup = gtk.ActionGroup("AppActions")
        self.add_actions( actiongroup, self.UI_ACTIONS)
        uimanager.insert_action_group(actiongroup, 0)
        uimanager.add_ui_from_string(self.UI_DEFINITION)
        menubar = uimanager.get_widget('/MenuBar')
        #self.toolbar = self.uimanager.get_widget('/ToolBar')
        mbox.pack_start(menubar, False)
        mbox.reorder_child(menubar, 0)
        #mbox.pack_start(self.toolbar, False)
        #mbox.reorder_child(self.toolbar, 1)

        tree.connect_after("cursor-changed", self.after_tree_move_cursor)
        window.connect("delete-event", gtk.main_quit)

        window.show_all()


    def load_classes(self, *fnames):
        allclasses = {}
        for fname in fnames:
            info = self.LayoutInfo(fname)
            self.hbox.pack_start( info.widget )
            for i in info.sizeFromName:
                allclasses[i] = 1
            self.layoutInfo.append(info)
        allclasses = allclasses.keys()
        allclasses.sort()
        for c in allclasses:
            self.model.append( (c,) )
        self.hbox.show_all()

    def after_tree_move_cursor(self, view, *args):
        sel = []
        def gather(model, path, iter):
            sel.append( model.get_path(iter) )
        view.get_selection().selected_foreach(gather)
        if len(sel):
            self.activate(view, sel[0])


    def activate(self, view, path):
        try:
            self.history[self.history_index] = path[0]
        except IndexError:
            self.history.append(path[0])
        self.history_index += 1
        self.show_view_path(view, path)


    def show_view_path(self, view, path):
        classname = self.model[path[0]][0]
        for layout in self.layoutInfo:
            layout.switch( classname, self.area_num_columns )


    def jump_to(self, name):
        print "JUMP", name
        name = name.split()
        if len(name) and name[0] in ("struct", "class"):
            name[1] = name[1].split("[")[0]
            if self.membersFromClass.has_key(name[1]):
                for i, row in enumerate(self.model):
                    if row[0] == name[1]:
                        self.activate(self.tree, (i,))
                        break


    def history_go(delta):
        pass
#        global history_index
#        print history_index, history
#        try:
#            item = history[history_index+delta]
#        except IndexError:
#            pass
#        else:
#            history_index += delta
#            show_view_path(tree, (item,) )

    def add_actions(self, actiongroup, actiondefs):
        """Connect actions to their methods.
        """
        normal_actions = []
        toggle_actions = []
        radio_actions = []
        for action in actiondefs:
            if len(action) == 3:
                normal_actions.append( action )
            else:
                if len(action)==5:
                    handler = getattr(self, "action_%s__activate"%action[0])
                    normal_actions.append( action + (handler,) )
                elif len(action)==6:
                    assert isinstance(action[-1], type(True))
                    handler = getattr(self, "action_%s__toggled"%action[0])
                    toggle_actions.append( action[:-1] + (handler,action[-1]) )
                elif len(action)==7:
                    assert isinstance(action[-2], type(0))
                    radio_actions.append( action )
                else:
                    print "**", len(action)
                    assert 0
        actiongroup.add_actions( normal_actions )
        actiongroup.add_toggle_actions( toggle_actions )
        for i in range(len(radio_actions)):
            if i+1==len(radio_actions) or radio_actions[i] != radio_actions[i+1]:
                handler = getattr(self, "action_%s__changed"%radio_actions[i][-1])
                actions = [a[:-1] for a in radio_actions]
                actiongroup.add_radio_actions( actions, -1, handler )
        for action in actiondefs:
            setattr(self, "action_"+action[0], actiongroup.get_action(action[0]))

    def action_open__activate(self, *args):
        fchooser = gtk.FileChooserDialog(
            parent=self.window.get_toplevel(),
            action=gtk.FILE_CHOOSER_ACTION_OPEN,
            buttons=(gtk.STOCK_CANCEL, gtk.RESPONSE_CANCEL, gtk.STOCK_OK, gtk.RESPONSE_OK) )
        if fchooser.run() == gtk.RESPONSE_OK:
            filename = fchooser.get_filename()
            self.load_classes(filename)
        fchooser.destroy()


    def action_quit__activate(self, *args):
        gtk.main_quit()


    def action_area_num_columns__changed(self, aold, anew):
        self.area_num_columns = anew.get_current_value()
        self.after_tree_move_cursor(self.tree)


    def main(self):
        gtk.main()

def main():
    app = ShowClass()
    app.load_classes(*sys.argv[1:])
    app.main()

if __name__=="__main__":
    main()

#
# Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20090216)
# 
# Confidential Information of Havok.  (C) Copyright 1999-2009
# Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
# Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
# rights, and intellectual property rights in the Havok software remain in
# Havok and/or its suppliers.
# 
# Use of this software for evaluation purposes is subject to and indicates
# acceptance of the End User licence Agreement for this product. A copy of
# the license is included with this software and is also available at www.havok.com/tryhavok.
# 
#
